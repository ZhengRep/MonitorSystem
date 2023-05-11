#include "SocketStream.h"

SocketStream::SocketStream(SocketIPv4* sock)
  : m_socket(sock)
{
  _ASSERT(m_socket != NULL);
}

SocketStream::~SocketStream()
{
}

size_t SocketStream::read(void* buf, size_t wanted) throw(IOException)
{
  if ((int)wanted < 0) {
    throw IOException(_T("Wanted size too big."));
  }

  return (size_t)m_socket->recv((char*)buf, (int)wanted);
}

size_t SocketStream::write(const void* buf, size_t size) throw(IOException)
{
  if ((int)size < 0) {
    throw IOException(_T("Size of buffer is too big."));
  }

  return (size_t)m_socket->send((char*)buf, (int)size);
}

void SocketStream::close()
{
  try {
    m_socket->shutdown(SD_BOTH);
  } catch (...) {
  }
  m_socket->close();
}

size_t SocketStream::available()
{
  return m_socket->available();
}
