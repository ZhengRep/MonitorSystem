#include "QueryConnectionCommandLine.h"
#include "Utils/StringParser.h"

const TCHAR QueryConnectionCommandLine::QUERY_CONNECTION[] = _T("-queryconnection");
const TCHAR QueryConnectionCommandLine::PEER_ADDR[] = _T("-peer");
const TCHAR QueryConnectionCommandLine::TIMEOUT[] = _T("-timeout");
const TCHAR QueryConnectionCommandLine::ACCEPT[] = _T("-accept");

QueryConnectionCommandLine::QueryConnectionCommandLine()
{
}

QueryConnectionCommandLine::~QueryConnectionCommandLine()
{
}

void QueryConnectionCommandLine::parse(const CommandLineArgs* commandLine) throw(Exception)
{
  CommandLineFormat format[] = {
    { QUERY_CONNECTION, NO_ARG },
    { PEER_ADDR, NEEDS_ARG},
    { TIMEOUT, NEEDS_ARG },
    { ACCEPT, NO_ARG }
  };

  if (!CommandLine::parse(format, sizeof(format) / sizeof(CommandLineFormat), commandLine)) {
    throw Exception(_T("Wrong command line format"));
  }

  if (!optionSpecified(QUERY_CONNECTION)) {
    throw Exception(_T("-queryconnection flag is not specified"));
  }

  StringStorage timeoutStr;

  if (optionSpecified(TIMEOUT, &timeoutStr) &&
    !StringParser::parseInt(timeoutStr.getString(), (int*)&m_timeout)) {
    throw Exception(_T("Invalid timeout"));
  }
}

bool QueryConnectionCommandLine::isTimeoutSpecified()
{
  return optionSpecified(TIMEOUT);
}

void QueryConnectionCommandLine::getPeerAddress(StringStorage* storage)
{
  optionSpecified(PEER_ADDR, storage);
}

bool QueryConnectionCommandLine::isDefaultActionAccept()
{
  return optionSpecified(ACCEPT);
}

DWORD QueryConnectionCommandLine::getTimeout()
{
  return m_timeout;
}
