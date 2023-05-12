#include "MntService.h"
#include "Utils/Macros.h"
#include "ServerCommandLine.h"
#include "vnc_server_app/NamingDefs.h"
#include "winSystem/SCMClient.h"
#include "winSystem/Environment.h"

const TCHAR MntService::SERVICE_COMMAND_LINE_KEY[] = _T("-service");

MntService::MntService(WinServiceEvents* winServiceEvents,
  NewConnectionEvents* newConnectionEvents)
  : Service(ServiceNames::SERVICE_NAME), m_mntServer(0),
  m_winServiceEvents(winServiceEvents),
  m_newConnectionEvents(newConnectionEvents),
  m_logServer(LogNames::LOG_PIPE_PUBLIC_NAME),
  m_clientLogger(LogNames::LOG_PIPE_PUBLIC_NAME, LogNames::SERVER_LOG_FILE_STUB_NAME)
{
}

MntService::~MntService()
{
}

void MntService::onStart()
{
  try {
    m_winServiceEvents->enable();
    // FIXME: Use real logger instead of zero.
    m_mntServer = new MntServer(true, m_newConnectionEvents, this, &m_clientLogger);
    m_mntServer->addListener(this);
    m_winServiceEvents->onSuccServiceStart();
  } catch (Exception& e) {
    m_winServiceEvents->onFailedServiceStart(&unmove(StringStorage(e.getMessage())));
  }
}

void MntService::main()
{
  m_shutdownEvent.waitForEvent();
  m_mntServer->removeListener(this);
  delete m_mntServer;
  m_mntServer = 0;

  m_winServiceEvents->onServiceStop();
}

void MntService::onStop()
{
  m_shutdownEvent.notify();
}

void MntService::onMntServerShutdown()
{
  MntService::stop();
}

void MntService::install()
{
  StringStorage binPath;

  MntService::getBinPath(&binPath);

  SCMClient scManager;

  scManager.installService(ServiceNames::SERVICE_NAME,
    ServiceNames::SERVICE_NAME_TO_DISPLAY,
    binPath.getString(), _T(""));
}

void MntService::remove()
{
  SCMClient scManager;

  scManager.removeService(ServiceNames::SERVICE_NAME);
}

void MntService::reinstall()
{
  try {
    remove();
  } catch (...) {}

  MntService::install();
}

void MntService::start(bool waitCompletion)
{
  SCMClient scManager;

  scManager.startService(ServiceNames::SERVICE_NAME, waitCompletion);
}

void MntService::stop(bool waitCompletion)
{
  SCMClient scManager;

  scManager.stopService(ServiceNames::SERVICE_NAME, waitCompletion);
}

bool MntService::getBinPath(StringStorage* binPath)
{
  StringStorage pathToServiceBinary;

  // Get executable folder first.
  if (!Environment::getCurrentModulePath(&pathToServiceBinary)) {
    return false;
  }

  // Create formatted binary path.
  binPath->format(_T("\"%s\" %s"),
    pathToServiceBinary.getString(),
    SERVICE_COMMAND_LINE_KEY);

  return true;
}

void MntService::onLogInit(const TCHAR* logDir, const TCHAR* fileName,
  unsigned char logLevel)
{
  size_t headerLineCount = m_clientLogger.getLogDumpSize();
  m_logServer.start(logDir, logLevel, headerLineCount);
  m_clientLogger.connect();
}

void MntService::onChangeLogProps(const TCHAR* newLogDir, unsigned char newLevel)
{
  m_logServer.changeLogProps(newLogDir, newLevel);
}
