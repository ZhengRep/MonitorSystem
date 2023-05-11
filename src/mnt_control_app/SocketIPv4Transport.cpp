#include "SocketIPv4Transport.h"
#include "network/socket/SocketStream.h"

SocketIPv4Transport::SocketIPv4Transport(SocketIPv4* socket)
  : m_socket(socket)
{
  m_stream = new SocketStream(m_socket);
}

SocketIPv4Transport::~SocketIPv4Transport()
{
  try { close(); } catch (...) {}

  delete m_stream;
  delete m_socket;
}

Channel* SocketIPv4Transport::getIOStream()
{
  if (m_socket->isBound()) {
    _ASSERT(FALSE);

    return 0;
  }

  return m_stream;
}

Transport* SocketIPv4Transport::accept()
{
  if (!m_socket->isBound()) {
    _ASSERT(FALSE);

    return 0;
  }

  return new SocketIPv4Transport(m_socket->accept());
}

void SocketIPv4Transport::close()
{
  try { m_socket->shutdown(SD_BOTH); } catch (...) {}

  m_socket->close();
}
