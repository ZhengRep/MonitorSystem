#pragma once
#include "Utils/CommonHeader.h"
#include "Utils/CommandLine.h"
#include "Utils/Exception.h"

class QueryConnectionCommandLine: public CommandLine
{
public:
  static const TCHAR QUERY_CONNECTION[];
  static const TCHAR PEER_ADDR[];
  static const TCHAR TIMEOUT[];
  static const TCHAR ACCEPT[];

public:
  QueryConnectionCommandLine();
  virtual ~QueryConnectionCommandLine();

  void parse(const CommandLineArgs* commandLine) throw(Exception);

  bool isTimeoutSpecified();

  void getPeerAddress(StringStorage* storage);
  bool isDefaultActionAccept();
  DWORD getTimeout();

protected:
  DWORD m_timeout;
};

