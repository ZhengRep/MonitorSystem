#include "SocketAddressIPv4.h"
#include "SocketException.h"
#include "Utils/AnsiStringStorage.h"
#include "thread/AutoLock.h"

#pragma warning(push)
#pragma warning(disable: 4996)

LocalMutex SocketAddressIPv4::s_resolveMutex;

SocketAddressIPv4::SocketAddressIPv4()
  : m_wsaStartup(1, 2)
{
  m_addr.s_addr = INADDR_ANY;
  m_port = 0;
}

SocketAddressIPv4::SocketAddressIPv4(sockaddr_in addr)
  : m_wsaStartup(1, 2)
{
  m_addr.s_addr = INADDR_ANY;
  m_port = 0;

  if (addr.sin_family != AF_INET) {
    throw SocketException(_T("The specified m_addr is not AF_INET family m_addr!"));
  }

  m_addr.s_addr = ntohl(addr.sin_addr.s_addr);
  m_port = ntohs(addr.sin_port);
}

SocketAddressIPv4::SocketAddressIPv4(const TCHAR* host, unsigned short port)
  : m_wsaStartup(1, 2)
{
  SocketAddressIPv4 sa = SocketAddressIPv4::resolve(host, port);

  this->m_addr = sa.m_addr;
  this->m_port = sa.m_port;
}

SocketAddressIPv4::SocketAddressIPv4(const SocketAddressIPv4& socketAddressIPv4)
  : m_wsaStartup(1, 2) 
{
  m_addr = socketAddressIPv4.m_addr;
  m_port = socketAddressIPv4.m_port;
}

SocketAddressIPv4& SocketAddressIPv4::operator=(const SocketAddressIPv4& socketAddressIPv4)
{
  if (this != &socketAddressIPv4) {
    this->m_addr = socketAddressIPv4.m_addr;
    this->m_port = socketAddressIPv4.m_port;
  }
  return *this;
}

socklen_t SocketAddressIPv4::getAddrLen() const
{
  return sizeof(struct sockaddr_in);
}

sockaddr_in SocketAddressIPv4::getSockAddr() const
{
  struct sockaddr_in saddr;

  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(m_addr.s_addr);
  saddr.sin_port = htons(m_port);

#ifndef _WIN32
  saddr.sin_len = sizeof(struct sockaddr_in);
#endif

  return saddr;
}

void SocketAddressIPv4::toString(StringStorage* address) const
{
  u_char b1 = m_addr.S_un.S_un_b.s_b4;
  u_char b2 = m_addr.S_un.S_un_b.s_b3;
  u_char b3 = m_addr.S_un.S_un_b.s_b2;
  u_char b4 = m_addr.S_un.S_un_b.s_b1;

  address->format(_T("%d.%d.%d.%d"), b1, b2, b3, b4);
}

unsigned short SocketAddressIPv4::getPort() const
{
  return m_port;
}

SocketAddressIPv4 SocketAddressIPv4::resolve(const TCHAR* host, unsigned short port) throw(SocketException)
{
  SocketAddressIPv4 resolvedAddress;

  StringStorage hostStorage(host);

  {
    AutoLock l(&s_resolveMutex);

    AnsiStringStorage hostAnsi(&hostStorage);

    hostent* hent = gethostbyname(hostAnsi.getString());
    if (hent == 0) {
      throw SocketException();
    }

    resolvedAddress.m_addr.S_un.S_addr = ntohl(*(u_long*)hent->h_addr_list[0]);
  }

  resolvedAddress.m_port = port;

  return resolvedAddress;
}

void getLocalIPAddrString(char* buffer, int buflen)
{
  char namebuf[256];

  if (gethostname(namebuf, 256) != 0) {
    strncpy(buffer, "Host name unavailable", buflen);
    return;
  };

  HOSTENT* ph = gethostbyname(namebuf);
  if (!ph) {
    strncpy(buffer, "IP address unavailable", buflen);
    return;
  };

  *buffer = '\0';
  char digtxt[5];
  for (int i = 0; ph->h_addr_list[i]; i++) {
    for (int j = 0; j < ph->h_length; j++) {
      sprintf(digtxt, "%d.", (unsigned char)ph->h_addr_list[i][j]);
      strncat(buffer, digtxt, (buflen - 1) - strlen(buffer));
    }
    buffer[strlen(buffer) - 1] = '\0';
    if (ph->h_addr_list[i + 1] != 0) {
      strncat(buffer, ", ", (buflen - 1) - strlen(buffer));
    }
  }
}

#pragma warning(pop)