#include "ControlServer.h"
#include "ControlClient.h"
#include "mnt_control_app/NamedPipeTransport.h"

ControlServer::ControlServer(PipeServer* pipeServer, RfbClientManager* rfbClientManager, LogWriter* log) throw(Exception)
  : m_authenticator(30000, 3),
  m_pipeServer(pipeServer),
  m_rfbClientManager(rfbClientManager),
  m_log(log)
{
  m_log->message(_T("%s"), _T("Control server started"));

  resume();
}

ControlServer::~ControlServer()
{
  m_log->message(_T("Destroying control server transport"));

  terminate();
  wait();

  try {
    m_pipeServer->close();
  } catch (Exception& ex) {
    m_log->error(_T("Failed to destroy control server transport with '%s' reason"), ex.getMessage());
  }

  delete m_pipeServer;

  // Unblock all client if it has been blocked by authenticator
  m_authenticator.breakAndDisableAuthentications();

  m_log->message(_T("%s"), _T("Control server stopped"));
}

void ControlServer::execute()
{
  try {
    while (!isTerminating()) {
      NamedPipe* pipe = m_pipeServer->accept();
      Transport* transport = new NamedPipeTransport(pipe);

      ControlClient* clientThread = new ControlClient(transport,
        m_rfbClientManager,
        &m_authenticator,
        pipe->getHandle(),
        m_log);

      clientThread->resume();

      m_threadCollector.addThread(clientThread);
    }
  } catch (Exception& ex) {
    m_log->error(_T("Exception on control server thread: %s"), ex.getMessage());
  }
}

void ControlServer::onTerminate()
{
  try { m_pipeServer->close(); } catch (...) {}
}
