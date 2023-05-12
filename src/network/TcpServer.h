#pragma once
#include "thread/Thread.h"
#include "socket/SocketIPv4.h"
#include "utils/Exception.h"

class TcpServer: public Thread
{
public:
  TcpServer(const TCHAR* bindHost,unsigned short bindPort,
    bool autoStart = false, bool lockAddr = false) throw(Exception);

  virtual ~TcpServer();
  const TCHAR* getBindHost() const;
  unsigned short getBindPort() const;

protected:
  virtual void start();
  virtual void onAcceptConnection(SocketIPv4* socket) = 0;

  virtual void execute();

private:
  SocketIPv4 m_listenSocket;
  StringStorage m_bindHost;
  unsigned short m_bindPort;
};

