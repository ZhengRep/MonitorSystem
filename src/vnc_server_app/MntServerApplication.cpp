#include "MntServerApplication.h"
#include "ServerCommandLine.h"
#include "thread/GlobalMutex.h"
#include "Utils/ResourceLoader.h"
#include "Utils/StringTable.h"
#include "vnc_server_app/NamingDefs.h"
#include "winSystem/WinCommandLineArgs.h"
#include "mnt_server/resource.h"
#include "winSystem/RegistryKey.h"

MntServerApplication::MntServerApplication(HINSTANCE hInstance,
  const TCHAR* windowClassName,
  const TCHAR* commandLine,
  NewConnectionEvents* newConnectionEvents)
  : WindowsApplication(hInstance, windowClassName),
  m_fileLogger(true),
  m_mntServer(0),
  m_commandLine(commandLine),
  m_newConnectionEvents(newConnectionEvents)
{
}

MntServerApplication::~MntServerApplication()
{
}

int MntServerApplication::run()
{
  // FIXME: May be an unhandled exception.
  // Check wrong command line and situation when we need to show help.

  try {
    ServerCommandLine parser;
    WinCommandLineArgs cmdArgs(m_commandLine.getString());
    if (!parser.parse(&cmdArgs) || parser.showHelp()) {
      throw Exception(_T("Wrong command line argument"));
    }
  } catch (...) {
    //MntServerHelp::showUsage();
    return 0;
  }


  GlobalMutex* appInstanceMutex;

  try {
    appInstanceMutex = new GlobalMutex(
      ServerApplicationNames::SERVER_INSTANCE_MUTEX_NAME, false, true);
  } catch (...) {
    MessageBox(0,
      StringTable::getString(IDS_SERVER_ALREADY_RUNNING),
      StringTable::getString(IDS_MBC_MNTSERVER), MB_OK | MB_ICONEXCLAMATION);
    return 1;
  }

  // check the HKLM\SOFTWARE\MNT\Server\ has ServiceOnly subsection and exit if found
  // to create the key and set acces rights run the PS script:
  // New-Item -Path HKLM:\SOFTWARE\MNT\Server -Name ServiceOnly
  // $ACL = Get-Acl HKLM:\SOFTWARE\MNT\Server\ServiceOnly
  // $AccessRule = new-object System.Security.AccessControl.RegistryAccessRule("Users", "ReadKey", "None", "None", "Allow")
  // $ACL.SetAccessRule($AccessRule)
  // $ACL | Set-Acl HKLM:\SOFTWARE\MNT\Server\ServiceOnly
  RegistryKey key(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\MNT\\Server\\ServiceOnly"), false);
  if (key.isOpened()) {
    MessageBox(0,
      _T("Couldn't run the server in Application mode"),
      _T("Server error"), MB_OK | MB_ICONEXCLAMATION);
    return 1;
  }

  // Start MNT server and MNT control application.
  try {
    m_mntServer = new MntServer(false, m_newConnectionEvents, this, &m_fileLogger);
    m_mntServer->addListener(this);
    m_mntControlRunner = new WsConfigRunner(&m_fileLogger);

    int exitCode = WindowsApplication::run();

    delete m_mntControlRunner;
    m_mntServer->removeListener(this);
    delete m_mntServer;
    delete appInstanceMutex;
    return exitCode;
  } catch (Exception& e) {
    StringStorage message;
    message.format(_T("Couldn't run the server: %s"), e.getMessage());
    MessageBox(0,
      message.getString(),
      _T("Server error"), MB_OK | MB_ICONEXCLAMATION);
    return 1;
  }
}

void MntServerApplication::onMntServerShutdown()
{
  WindowsApplication::shutdown();
}

void MntServerApplication::onLogInit(const TCHAR* logDir, const TCHAR* fileName,
  unsigned char logLevel)
{
  m_fileLogger.init(logDir, fileName, logLevel);
  m_fileLogger.storeHeader();
}

void MntServerApplication::onChangeLogProps(const TCHAR* newLogDir, unsigned char newLevel)
{
  m_fileLogger.changeLogProps(newLogDir, newLevel);
}
