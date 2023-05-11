#include "WindowsSocket.h"

bool WindowsSocket::m_isStarted = false;

void WindowsSocket::startup(BYTE loVer, BYTE hiVer) throw(Exception)
{
  if (m_isStarted) {
    throw Exception(_T("WindowsSocket already initialized."));
  }

  WSAData wsaData;

  if (WSAStartup(MAKEWORD(loVer, hiVer), &wsaData) != 0) {
    throw Exception(_T("Failed to initialize WindowsSocket."));
  }

  m_isStarted = true;
}

void WindowsSocket::cleanup() throw(Exception)
{
  if (!m_isStarted) {
    throw Exception(_T("WindowsSocket don't initialized."));
  }

  m_isStarted = false;

  if (WSACleanup() == SOCKET_ERROR) {
    throw Exception(_T("Failed to deinitialize WindowsSocket."));
  }
}
