#include "ListenLogServer.h"

ListenLogServer::ListenLogServer(const TCHAR* publicPipeName,
  ConnectionListener* connListener)
  : m_connListener(connListener)
{
  m_pipeSecurity.shareToAllUsers();
  const unsigned int logBufferSizeForServiceChannel = 0x10000;
  m_listenPipe = new PipeServer(publicPipeName, logBufferSizeForServiceChannel);
  resume();
}

ListenLogServer::~ListenLogServer()
{
  terminate();
  m_timer.notify();
  m_listenPipe->close();
  wait();
  delete m_listenPipe;
}

void ListenLogServer::execute()
{
  while (!isTerminating()) {
    try {
      NamedPipe* pipe = m_listenPipe->accept();
      m_connListener->onNewConnection(pipe);
    } catch (...) {
      if (!isTerminating()) {
        m_timer.waitForEvent(1000); // Pause
      }
    }
  }
}
