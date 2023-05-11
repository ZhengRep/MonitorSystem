#pragma once
#include "RfbClientManager.h"
#include "ControlAppAuthenticator.h"
#include "network/TcpServer.h"
#include "winSystem/PipeServer.h"
#include "thread/ThreadCollector.h"

class ControlServer: private Thread
{
public:
  ControlServer(PipeServer* pipeServer,RfbClientManager* rfbClientManager, 
    LogWriter* log) throw(Exception);
  virtual ~ControlServer();

protected:
  virtual void execute();
  virtual void onTerminate();

private:
  ControlAppAuthenticator m_authenticator;
  ThreadCollector m_threadCollector;

  PipeServer* m_pipeServer;
  RfbClientManager* m_rfbClientManager;

  LogWriter* m_log;
};

