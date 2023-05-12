#include "ServiceControlApplication.h"
#include "ServiceControlCommandLine.h"
#include "Utils/ResourceLoader.h"
#include "Utils/StringTable.h"
#include "vnc_server_app/NamingDefs.h"
#include "mnt_server/resource.h"
#include "mnt_control_app/ControlCommandLine.h"
#include "WsConfigRunner.h"
#include "winSystem/Registry.h"
#include "winSystem/RegistryKey.h"
#include "winSystem/Environment.h"
#include "winSystem/Shell.h"
#include "winSystem/SCMClient.h"
#include "winSystem/WinCommandLineArgs.h"

ServiceControlApplication::ServiceControlApplication(HINSTANCE hInstance,
  const TCHAR* windowClassName,
  const TCHAR* commandLine)
  : WindowsApplication(hInstance, windowClassName),
  m_commandLine(commandLine)
{
}

ServiceControlApplication::~ServiceControlApplication()
{
}

int ServiceControlApplication::run()
{
  
  const int RET_OK = 0;
  const int RET_ERR = 1;

  ServiceControlCommandLine cmdLine;
  try {
    WinCommandLineArgs cmdArgs(m_commandLine.getString());
    cmdLine.parse(&cmdArgs);
  } catch (Exception&) {
    //MntServerHelp::showUsage();
    return RET_ERR;
  }

  bool success = false;

  if (!IsUserAnAdmin() && !cmdLine.beSilent() && !cmdLine.dontElevate()) {
    // The privileges are insufficient and there were no -silent/-dontelevate
    // options so we can request privilege elevation and run another
    // "elevated" instance of the same program, with additional -dontelevate
    // option in the command line.
    try {
      runElevatedInstance();
      success = true;
    } catch (SystemException& sysEx) {
      if (sysEx.getErrorCode() != ERROR_CANCELLED) {
        reportError(&cmdLine, sysEx.getMessage());
      }
    }
  } else {
    // Do the work in current instance, do not request privilege elevation.
    try {
      executeCommand(&cmdLine);
      success = true;
    } catch (SCMClientException& scmEx) {
      reportError(&cmdLine, &scmEx);
    } catch (SystemException& servEx) {
      reportError(&cmdLine, &servEx);
    } catch (Exception& ex) {
      _ASSERT(FALSE);
      reportError(&cmdLine, ex.getMessage());
    }
  }

  // After trying to start the service, regardless of the result, run the
  // control interface with non-elevated privileges. Make sure to skip this
  // step if there was a -dontelevate option (so we are a child process).
  if (cmdLine.startRequested() && !cmdLine.dontElevate()) {
    try {
      WsConfigRunner mntcontrol(0, true);
    } catch (...) {}
  }

  return success ? RET_OK : RET_ERR;
}


void ServiceControlApplication::runElevatedInstance() const
{
  StringStorage executablePath;
  Environment::getCurrentModulePath(&executablePath);

  StringStorage commandLine;
  commandLine.format(_T("%s %s"), m_commandLine.getString(),
    ServiceControlCommandLine::DONT_ELEVATE);

  Shell::runAsAdmin(executablePath.getString(), commandLine.getString());
}

void ServiceControlApplication::executeCommand(const ServiceControlCommandLine* cmdLine) const
{
  if (cmdLine->installationRequested()) {
    MntService::install();
    setMntControlStartEntry();
    reportSuccess(cmdLine);
  } else if (cmdLine->removalRequested()) {
    MntService::remove();
    removeMntControlStartEntry();
    reportSuccess(cmdLine);
  } else if (cmdLine->reinstallRequested()) {
    MntService::reinstall();
    setMntControlStartEntry();
    reportSuccess(cmdLine);
  } else if (cmdLine->startRequested()) {
    MntService::start(true);
  } else if (cmdLine->stopRequested()) {
    MntService::stop(true);
  }
}

void ServiceControlApplication::setMntControlStartEntry() const
{
  // Prepare tvncontrol start command.
  StringStorage executablePath;
  Environment::getCurrentModulePath(&executablePath);
  StringStorage pathToTvnControl;
  pathToTvnControl.format(_T("\"%s\" %s %s"),
    executablePath.getString(),
    ControlCommandLine::CONTROL_SERVICE,
    ControlCommandLine::SLAVE_MODE);

  // Write registry entry.
  RegistryKey runKey(Registry::getCurrentLocalMachineKey(),
    _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
    false);
  runKey.setValueAsString(ServiceNames::MNTCONTROL_START_REGISTRY_ENTRY_NAME,
    pathToTvnControl.getString());
}

void ServiceControlApplication::removeMntControlStartEntry() const
{
  RegistryKey runKey(Registry::getCurrentLocalMachineKey(),
    _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
    false);
  runKey.deleteValue(ServiceNames::MNTCONTROL_START_REGISTRY_ENTRY_NAME);
}

void ServiceControlApplication::reportError(const ServiceControlCommandLine* cmdLine,
  const SCMClientException* ex) const
{
  StringStorage errorMessage;

  switch (ex->getSCMErrorCode()) {
  case SCMClientException::ERROR_ALREADY_RUNNING:
    errorMessage.setString(StringTable::getString(IDS_SERVICE_ALREADY_RUNNING));
    break;
  case SCMClientException::ERROR_ALREADY_STOPPED:
    errorMessage.setString(StringTable::getString(IDS_SERVICE_ALREADY_STOPPED));
    break;
  case SCMClientException::ERROR_START_TIMEOUT:
    errorMessage.setString(StringTable::getString(IDS_SERVICE_START_TIMEOUT));
    break;
  case SCMClientException::ERROR_STOP_TIMEOUT:
    errorMessage.setString(StringTable::getString(IDS_SERVICE_STOP_TIMEOUT));
    break;
  default:
    errorMessage.setString(ex->getMessage());
  }

  reportError(cmdLine, errorMessage.getString());
}

void ServiceControlApplication::reportError(const ServiceControlCommandLine* cmdLine,
  const SystemException* ex) const
{
  StringStorage errorMessage;

  switch (ex->getErrorCode()) {
  case ERROR_SERVICE_DOES_NOT_EXIST:
    errorMessage.setString(StringTable::getString(IDS_1060_ERROR_DESCRIPTION));
    break;
  case ERROR_SERVICE_EXISTS:
    errorMessage.setString(StringTable::getString(IDS_1073_ERROR_DESCRIPTION));
    break;
  default:
    errorMessage.setString(ex->getMessage());
  }

  reportError(cmdLine, errorMessage.getString());
}

void ServiceControlApplication::reportError(const ServiceControlCommandLine* cmdLine,
  const TCHAR* errorMessage) const
{
  UINT stringId = 0;

  if (cmdLine->installationRequested()) {
    stringId = IDS_FAILED_TO_INSTALL_SERVICE_FORMAT;
  } else if (cmdLine->removalRequested()) {
    stringId = IDS_FAILED_TO_REMOVE_SERVICE_FORMAT;
  } else if (cmdLine->reinstallRequested()) {
    stringId = IDS_FAILED_TO_INSTALL_SERVICE_FORMAT;
  } else if (cmdLine->startRequested()) {
    stringId = IDS_FAILED_TO_START_SERVICE_FORMAT;
  } else if (cmdLine->stopRequested()) {
    stringId = IDS_FAILED_TO_STOP_SERVICE_FORMAT;
  } else {
    _ASSERT(FALSE);
    return;
  }

  if (!cmdLine->beSilent()) {
    const TCHAR* caption = StringTable::getString(IDS_MBC_MNTSERVER);
    StringStorage text;
    text.format(StringTable::getString(stringId), errorMessage);
    MessageBox(NULL, text.getString(), caption, MB_OK | MB_ICONERROR);
  }
}

void ServiceControlApplication::reportSuccess(const ServiceControlCommandLine* cmdLine) const
{
  UINT stringId = 0;

  if (cmdLine->installationRequested()) {
    stringId = IDS_SERVICE_INSTALLED;
  } else if (cmdLine->removalRequested()) {
    stringId = IDS_SERVICE_REMOVED;
  } else if (cmdLine->reinstallRequested()) {
    stringId = IDS_SERVICE_REINSTALLED;
  } else {
    _ASSERT(FALSE);
    return;
  }

  if (!cmdLine->beSilent()) {
    const TCHAR* caption = StringTable::getString(IDS_MBC_MNTSERVER);
    const TCHAR* text = StringTable::getString(stringId);
    MessageBox(NULL, text, caption, MB_OK | MB_ICONINFORMATION);
  }
}
