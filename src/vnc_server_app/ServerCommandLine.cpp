#include "ServerCommandLine.h"

ServerCommandLine::ServerCommandLine()
{
}

ServerCommandLine::~ServerCommandLine()
{
}

bool ServerCommandLine::parse(const CommandLineArgs* cmdArgs)
{
  CommandLineFormat format[] = {
    { _T("-help"), NO_ARG },
    { _T("-h"), NO_ARG },
    { _T("-?"), NO_ARG },
    { _T("/help"), NO_ARG },
    { _T("/h"), NO_ARG },
    { _T("/?"), NO_ARG },
    { _T("-run"), NO_ARG }
  };

  if (!CommandLine::parse(format, sizeof(format) / sizeof(CommandLineFormat), cmdArgs)) {
    return false;
  }
  if (showHelp() && optionSpecified(_T("-run"))) {
    return false;
  }

  return true;
}

bool ServerCommandLine::showHelp()
{
  return optionSpecified(_T("-help")) || optionSpecified(_T("-h")) ||
    optionSpecified(_T("-?")) || optionSpecified(_T("/help")) ||
    optionSpecified(_T("/h")) || optionSpecified(_T("/?"));
}