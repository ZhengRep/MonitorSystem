#include "ServiceControlCommandLine.h"

const TCHAR ServiceControlCommandLine::INSTALL_SERVICE[] = _T("-install");
const TCHAR ServiceControlCommandLine::REMOVE_SERVICE[] = _T("-remove");
const TCHAR ServiceControlCommandLine::REINSTALL_SERVICE[] = _T("-reinstall");
const TCHAR ServiceControlCommandLine::START_SERVICE[] = _T("-start");
const TCHAR ServiceControlCommandLine::STOP_SERVICE[] = _T("-stop");

const TCHAR ServiceControlCommandLine::DONT_ELEVATE[] = _T("-dontelevate");
const TCHAR ServiceControlCommandLine::SILENT[] = _T("-silent");

ServiceControlCommandLine::ServiceControlCommandLine()
{
}

ServiceControlCommandLine::~ServiceControlCommandLine()
{
}

void ServiceControlCommandLine::parse(const CommandLineArgs* cmdArgs)
{
  CommandLineFormat format[] = {
    { INSTALL_SERVICE, NO_ARG },
    { REMOVE_SERVICE, NO_ARG },
    { REINSTALL_SERVICE, NO_ARG },
    { START_SERVICE, NO_ARG },
    { STOP_SERVICE, NO_ARG },
    { SILENT, NO_ARG },
    { DONT_ELEVATE, NO_ARG }
  };

  if (!CommandLine::parse(format,
    sizeof(format) / sizeof(CommandLineFormat),
    cmdArgs)) {
    throw Exception(_T("invalid command line"));
  } // if cannot parse it.

  // Check additional rules.

  if (keySpecified(SILENT)) {
    if (m_foundKeys.size() != (dontElevate() ? 3 : 2)) {
      throw Exception(_T("-silent key can be used only when one command specified"));
    }
  } else if (m_foundKeys.size() != (dontElevate() ? 2 : 1)) {
    throw Exception(_T("only one service command can be specified"));
  }
}

bool ServiceControlCommandLine::keySpecified(const TCHAR* key) const
{
  return optionSpecified(key);
}

bool ServiceControlCommandLine::installationRequested() const
{
  return keySpecified(INSTALL_SERVICE);
}

bool ServiceControlCommandLine::removalRequested() const
{
  return keySpecified(REMOVE_SERVICE);
}

bool ServiceControlCommandLine::reinstallRequested() const
{
  return keySpecified(REINSTALL_SERVICE);
}

bool ServiceControlCommandLine::startRequested() const
{
  return keySpecified(START_SERVICE);
}

bool ServiceControlCommandLine::stopRequested() const
{
  return keySpecified(STOP_SERVICE);
}

bool ServiceControlCommandLine::beSilent() const
{
  return keySpecified(SILENT);
}

bool ServiceControlCommandLine::dontElevate() const
{
  return keySpecified(DONT_ELEVATE);
}
