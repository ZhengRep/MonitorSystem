#include "NamedPipeTransport.h"

NamedPipeTransport::NamedPipeTransport(NamedPipe* client)
  : m_pipe(client), m_pipeServer(0)
{
}

NamedPipeTransport::NamedPipeTransport(PipeServer* server)
  : m_pipe(0), m_pipeServer(server)
{
}

NamedPipeTransport::~NamedPipeTransport()
{
  if (m_pipe != 0) {
    delete m_pipe;
  }
  if (m_pipeServer != 0) {
    delete m_pipeServer;
  }
}

Channel* NamedPipeTransport::getIOStream()
{
  _ASSERT(m_pipe != 0);

  return m_pipe;
}

Transport* NamedPipeTransport::accept() throw(Exception)
{
  _ASSERT(m_pipeServer != 0);

  return new NamedPipeTransport(m_pipeServer->accept());
}

void NamedPipeTransport::close() throw(Exception)
{
  if (m_pipe != 0) {
    m_pipe->close();
  }
  if (m_pipeServer != 0) {
    m_pipeServer->close();
  }
}
