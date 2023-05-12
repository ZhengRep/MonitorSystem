#include "MntServer.h"
#include "mnt_control_app/ControlPipeName.h"
#include "mnt_server_app/NamingDefs.h"
#include "mnt_server/BuildTime.h"
#include "network/socket/WindowsSocket.h"
#include "utils/AnsiStringStorage.h"
#include "mnt_server/resource.h"
#include "win_system/Process.h"
#include "AdditionalActionApplication.h"
#include "win_system/Environment.h"
#include "win_system/CurrentConsoleProcess.h"

#pragma comment(lib, "Winmm.lib")

MntServer::MntServer(bool runsInServiceContext, NewConnectionEvents* newConnectionEvents, LogInitListener* logInitListener, Logger* logger)
  : Singleton<MntServer>(),
  ListenerContainer<MntServerListener*>(),
  m_runAsService(runsInServiceContext),
  m_logInitListener(logInitListener),
  m_rfbClientManager(0),
  m_config(runsInServiceContext),
  m_log(logger),
  m_contextSwitchResolution(1)
{
  m_log.message(_T("%s Build on %s"),
    ProductNames::SERVER_PRODUCT_NAME, BuildTime::DATE);
    //ProductNames::SERVER_PRODUCT_NAME, _T("TestTime"));

  // Initialize configuration.
  Configurator* configurator = Configurator::getInstance();
  configurator->load();
  m_srvConfig = Configurator::getInstance()->getServerConfig();

  try {
    StringStorage logDir;
    m_srvConfig->getLogFileDir(&logDir);
    unsigned char logLevel = m_srvConfig->getLogLevel();
    m_logInitListener->onLogInit(logDir.getString(), LogNames::SERVER_LOG_FILE_STUB_NAME, logLevel);

  } catch (...) {
    // A log error must not be a reason that stop the server.
  }

  // Initialize windows sockets.

  m_log.info(_T("Initialize WinSock"));

  try {
    WindowsSocket::startup(2, 1);
  } catch (Exception& ex) {
    m_log.interror(_T("%s"), ex.getMessage());
  }

  DesktopFactory* desktopFactory = 0;
  if (runsInServiceContext) {
    desktopFactory = &m_serviceDesktopFactory;
  } else {
    desktopFactory = &m_applicationDesktopFactory;
  }

  m_rfbClientManager = new RfbClientManager(0, newConnectionEvents, &m_log, desktopFactory);

  m_rfbClientManager->addListener(this);

  Configurator::getInstance()->addListener(this);

  {
    AutoLock l(&m_mutex);

    restartMainRfbServer();
    //(void)m_extraRfbServers.reload(m_runAsService, m_rfbClientManager);
    restartControlServer();
  }
}

MntServer::~MntServer()
{
  Configurator::getInstance()->removeListener(this);

  stopControlServer();
  stopMainRfbServer();

  ZombieKiller* zombieKiller = ZombieKiller::getInstance();

  zombieKiller->killAllZombies();

  m_rfbClientManager->removeListener(this);

  delete m_rfbClientManager;

  m_log.info(_T("Shutdown WinSock"));

  try {
    WindowsSocket::cleanup();
  } catch (Exception& ex) {
    m_log.error(_T("%s"), ex.getMessage());
  }
}

void MntServer::getServerInfo(MntServerInfo* info)
{
  bool rfbServerListening = true;
  {
    AutoLock l(&m_mutex);
    rfbServerListening = m_rfbServer != 0;
  }

  StringStorage statusString;

  bool mntAuthEnabled = m_srvConfig->isUsingAuthentication();
  bool noMntPasswords = !m_srvConfig->hasPrimaryPassword() && !m_srvConfig->hasReadOnlyPassword();
  // Determinates that main rfb server cannot accept connection in case of passwords problem.
  bool mntPasswordsError = mntAuthEnabled && noMntPasswords;

  if (rfbServerListening) {
    if (mntPasswordsError) {
      statusString = StringTable::getString(IDS_NO_PASSWORDS_SET);
    } else {
      char localAddressString[1024];
      getLocalIPAddrString(localAddressString, 1024);
      AnsiStringStorage ansiString(localAddressString);
      ansiString.toStringStorage(&statusString);

      if (!mntAuthEnabled) {
        statusString.appendString(StringTable::getString(IDS_NO_AUTH_STATUS));
      } // if no auth enabled.
    } // accepting connections and no problem with passwords.
  } else {
    statusString = StringTable::getString(IDS_SERVER_NOT_LISTENING);
  } // not accepting connections.

  UINT stringId = m_runAsService ? IDS_MNTSERVER_SERVICE : IDS_MNTSERVER_APP;

  info->m_statusText.format(_T("%s - %s"),
    StringTable::getString(stringId),
    statusString.getString());
  info->m_acceptFlag = rfbServerListening && !mntPasswordsError;
  info->m_serviceFlag = m_runAsService;
}

void MntServer::onConfigReload(ServerConfig* serverConfig)
{
  // Start/stop/restart RFB servers if needed.
  {
    AutoLock l(&m_mutex);

    bool toggleMainRfbServer = m_srvConfig->isAcceptingRfbConnections() != (m_rfbServer != 0);
    bool changeMainRfbPort = m_rfbServer != 0 && (m_srvConfig->getRfbPort() != (int)m_rfbServer->getBindPort());

    const TCHAR* bindHost = m_srvConfig->isOnlyLoopbackConnectionsAllowed() ? _T("localhost") : _T("0.0.0.0");
    bool changeBindHost = m_rfbServer != 0 && _tcscmp(m_rfbServer->getBindHost(), bindHost) != 0;

    if (toggleMainRfbServer ||
      changeMainRfbPort ||
      changeBindHost) {
      restartMainRfbServer();
    }

    //(void)m_extraRfbServers.reload(m_runAsService, m_rfbClientManager);
  }

  changeLogProps();
}

void MntServer::generateExternalShutdownSignal()
{
  AutoLock l(&m_listeners);

  vector<MntServerListener*>::iterator it;
  for (it = m_listeners.begin(); it != m_listeners.end(); it++) {
    MntServerListener* each = *it;

    each->onMntServerShutdown();
  } // for all listeners.
}

bool MntServer::isRunningAsService() const
{
  return m_runAsService;
}

void MntServer::afterFirstClientConnect()
{
  if (timeBeginPeriod(m_contextSwitchResolution) == TIMERR_NOERROR) {
    m_log.message(_T("Set context switch resolution: %d ms"), m_contextSwitchResolution);
  } else {
    m_log.message(_T("Can't change context switch resolution to: %d ms"), m_contextSwitchResolution);
  }
}

void MntServer::afterLastClientDisconnect()
{
  m_log.message(_T("Restore context switch resolution"));
  timeEndPeriod(m_contextSwitchResolution);

  ServerConfig::DisconnectAction action = m_srvConfig->getDisconnectAction();

  // Disconnect action must be executed in process on interactive user session to take effect.
  // Now, choose application keys for specified action.

  StringStorage keys;

  switch (action) {
  case ServerConfig::DA_LOCK_WORKSTATION:
    keys.format(_T("%s"), AdditionalActionApplication::LOCK_WORKSTATION_KEY);
    break;
  case ServerConfig::DA_LOGOUT_WORKSTATION:
    keys.format(_T("%s"), AdditionalActionApplication::LOGOUT_KEY);
    break;
  default:
    return;
  }

  Process* process;

  // Choose how to start process.
  StringStorage thisModulePath;
  Environment::getCurrentModulePath(&thisModulePath);
  thisModulePath.quoteSelf();
  if (isRunningAsService()) {
    bool connectToRdp = m_srvConfig->getConnectToRdpFlag();
    process = new CurrentConsoleProcess(&m_log, connectToRdp, thisModulePath.getString(),
      keys.getString());
  } else {
    process = new Process(thisModulePath.getString(), keys.getString());
  }

  m_log.message(_T("Execute disconnect action in separate process"));

  try {
    process->start();
  } catch (SystemException& ex) {
    m_log.error(_T("Failed to start application: \"%s\""), ex.getMessage());
  }

  delete process;
}

void MntServer::restartHttpServer()
{
}

void MntServer::restartControlServer()
{
  stopControlServer();

  m_log.message(_T("Starting control server"));

  try {
    StringStorage pipeName;
    ControlPipeName::createPipeName(isRunningAsService(), &pipeName, &m_log);

    SecurityAttributes* pipeSecurity = new SecurityAttributes();
    pipeSecurity->setInheritable();
    pipeSecurity->shareToAllUsers();

    const unsigned int maxControlServerPipeBufferSize = 0x10000;
    PipeServer* pipeServer = new PipeServer(pipeName.getString(), maxControlServerPipeBufferSize, pipeSecurity);
    m_controlServer = new ControlServer(pipeServer, m_rfbClientManager, &m_log);
  } catch (Exception& ex) {
    m_log.error(_T("Failed to start control server: \"%s\""), ex.getMessage());
  }
}

void MntServer::restartMainRfbServer()
{
  stopMainRfbServer();

  if (!m_srvConfig->isAcceptingRfbConnections()) {
    return;
  }

  const TCHAR* bindHost = m_srvConfig->isOnlyLoopbackConnectionsAllowed() ? _T("localhost") : _T("0.0.0.0");
  unsigned short bindPort = m_srvConfig->getRfbPort();

  m_log.message(_T("Starting main RFB server"));

  try {
    m_rfbServer = new RfbServer(bindHost, bindPort, m_rfbClientManager, m_runAsService, &m_log);
  } catch (Exception& ex) {
    m_log.error(_T("Failed to start main RFB server: \"%s\""), ex.getMessage());
  }
}

void MntServer::stopHttpServer()
{
}

void MntServer::stopControlServer()
{
  stopMainRfbServer();

  if (!m_srvConfig->isAcceptingRfbConnections()) {
    return;
  }

  const TCHAR* bindHost = m_srvConfig->isOnlyLoopbackConnectionsAllowed() ? _T("localhost") : _T("0.0.0.0");
  unsigned short bindPort = m_srvConfig->getRfbPort();

  m_log.message(_T("Starting main RFB server"));

  try {
    m_rfbServer = new RfbServer(bindHost, bindPort, m_rfbClientManager, m_runAsService, &m_log);
  } catch (Exception& ex) {
    m_log.error(_T("Failed to start main RFB server: \"%s\""), ex.getMessage());
  }
}

void MntServer::stopMainRfbServer()
{
  m_log.message(_T("Stopping main RFB server"));

  RfbServer* rfbServer = 0;
  {
    AutoLock l(&m_mutex);
    rfbServer = m_rfbServer;
    m_rfbServer = 0;
  }
  if (rfbServer != 0) {
    delete rfbServer;
  }
}

void MntServer::changeLogProps()
{
  StringStorage logDir;
  unsigned char logLevel;
  {
    AutoLock al(&m_mutex);
    m_srvConfig->getLogFileDir(&logDir);
    logLevel = m_srvConfig->getLogLevel();
  }
  m_logInitListener->onChangeLogProps(logDir.getString(), logLevel);
}
