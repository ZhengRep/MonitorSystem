#pragma once
#include "Utils/CommonHeader.h"
#include "NamedPipe.h"
#include "thread/LocalMutex.h"
#include "SecurityAttributes.h"

class PipeServer
{
public:
  PipeServer(const TCHAR* name, unsigned int bufferSize, SecurityAttributes* secAttr = 0, DWORD milliseconds = INFINITE) throw (Exception);
  virtual ~PipeServer();
  NamedPipe* accept() throw (Exception);
  void close() throw(Exception);
  void waitForConnect(DWORD milliseconds = INFINITE);
  virtual void closeConnection();

private:
  void createServerPipe() throw (Exception);

private:
  StringStorage m_pipeName;
  WindowsEvent m_winEvent;
  DWORD m_milliseconds;
  SecurityAttributes* m_secAttr;
  HANDLE m_serverPipe;
  unsigned int m_bufferSize;
};

