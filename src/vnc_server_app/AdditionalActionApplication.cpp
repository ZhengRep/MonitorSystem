#include "AdditionalActionApplication.h"
#include "winSystem/Workstation.h"
#include "winSystem/WinCommandLineArgs.h"
#include "Utils/CommandLine.h"

const TCHAR AdditionalActionApplication::LOCK_WORKSTATION_KEY[] = _T("-lockworkstation");
const TCHAR AdditionalActionApplication::LOGOUT_KEY[] = _T("-logout");

AdditionalActionApplication::AdditionalActionApplication(HINSTANCE hInstance,
  const TCHAR* windowClassName,
  const TCHAR* commandLine)
  : LocalWindowsApplication(hInstance, windowClassName),
  m_commandLine(commandLine)
{
}

AdditionalActionApplication::~AdditionalActionApplication()
{
}

int AdditionalActionApplication::run()
{
  CommandLine args;

  CommandLineFormat format[] = {
    { LOCK_WORKSTATION_KEY, NO_ARG },
    { LOGOUT_KEY, NO_ARG }
  };

  try {

    WinCommandLineArgs cmdArgs(m_commandLine.getString());
    if (!args.parse(format,
      sizeof(format) / sizeof(CommandLineFormat),
      &cmdArgs)) {
    }
  } catch (...) {
    //show help usage
    return 0;
  }

  try {
    if (args.optionSpecified(LOCK_WORKSTATION_KEY)) {
      Workstation::lock();
    } else if (args.optionSpecified(LOGOUT_KEY)) {
      Workstation::logOff();
    }
  } catch (SystemException& sysEx) {
    return sysEx.getErrorCode();
  }
  return 0;
}
