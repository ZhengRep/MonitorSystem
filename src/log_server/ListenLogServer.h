#pragma once
#include "thread/Thread.h"
#include "win_system/PipeServer.h"
#include "ConnectionListener.h"

class ListenLogServer : private Thread
{
public:
  ListenLogServer(const TCHAR* publicPipeName,
    ConnectionListener* connListener);
  virtual ~ListenLogServer();

private:
  virtual void execute();

  PipeServer* m_listenPipe;
  SecurityAttributes m_pipeSecurity;

  ConnectionListener* m_connListener;
  WindowsEvent m_timer;
};

