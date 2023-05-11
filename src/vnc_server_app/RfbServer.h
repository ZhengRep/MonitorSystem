#pragma once
#include "RfbClientManager.h"
#include "thread/Thread.h"
#include "region/Rect.h"
#include "network/TcpServer.h"

class RfbServer: public TcpServer
{
public:
  RfbServer(const TCHAR* bindHost, unsigned short bindPort,
    RfbClientManager* clientManager, bool lockAddr,
    LogWriter* log,const Rect* viewPort = 0) throw(Exception);
  virtual ~RfbServer();

protected:
  virtual void onAcceptConnection(SocketIPv4* socket);

  RfbClientManager* m_clientManager;
  ViewPortState m_viewPort;
  LogWriter* m_log;
};

