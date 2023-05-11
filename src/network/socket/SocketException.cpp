#include "SocketException.h"

SocketException::SocketException()
  :Exception()
{
  setErrno(WSAGetLastError());
}

SocketException::SocketException(int error)
  :Exception()
{
  setErrno(error);
}

SocketException::SocketException(const TCHAR* message)
  :Exception(message), m_errno(0)
{
}

SocketException::~SocketException()
{
}

void SocketException::setErrno(int error)
{
  m_errno = error;
  TCHAR buffer[1024];

  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, m_errno, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR)&buffer[0], sizeof(buffer), NULL);

  m_message.setString(&buffer[0]);
}
