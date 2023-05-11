#include "TcpServer.h"

TcpServer::TcpServer(const TCHAR* bindHost, unsigned short bindPort, bool autoStart, bool lockAddr) throw(Exception)
  : m_bindHost(bindHost), m_bindPort(bindPort)
{
  SocketAddressIPv4 bindAddr = SocketAddressIPv4::resolve(bindHost, bindPort);

  if (lockAddr) {
    m_listenSocket.setExclusiveAddrUse();
  }

  m_listenSocket.bind(bindAddr);
  m_listenSocket.listen(10);

  if (autoStart) {
    start();
  }
}

TcpServer::~TcpServer()
{
  try { m_listenSocket.shutdown(SD_BOTH); } catch (...) {}
  try { m_listenSocket.close(); } catch (...) {}

  if (isActive()) {
    Thread::terminate();
    Thread::wait();
  }
}

const TCHAR* TcpServer::getBindHost() const
{
  return m_bindHost.getString();
}

unsigned short TcpServer::getBindPort() const
{
  return m_bindPort;
}

void TcpServer::start()
{
  resume();
}

void TcpServer::execute()
{
  while (!isTerminating()) {
    SocketIPv4* clientSocket = NULL;

    try {
      clientSocket = m_listenSocket.accept();
    } catch (...) {
      clientSocket = NULL;
    }

    if (clientSocket != NULL) {
      onAcceptConnection(clientSocket);
    } else {
      break;
    }
  }
}
