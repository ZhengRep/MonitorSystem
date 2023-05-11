#pragma once
#include "Utils/CommonHeader.h"
#include "thread/LocalMutex.h"
#include "SocketException.h"
#include "winSystem/WsaStartup.h"

void getLocalIPAddrString(char* buffer, int buflen);

class SocketAddressIPv4
{
public:
  SocketAddressIPv4();
  SocketAddressIPv4(sockaddr_in);
  SocketAddressIPv4(const TCHAR* host, unsigned short port);
  SocketAddressIPv4(const SocketAddressIPv4& socketAddressIPv4);
  SocketAddressIPv4& operator=(const SocketAddressIPv4& socketAddressIPv4);

  socklen_t getAddrLen() const;
  sockaddr_in getSockAddr() const;
  void toString(StringStorage* address) const;
  unsigned short getPort() const;
  static SocketAddressIPv4 resolve(const TCHAR* host, unsigned short port) throw(SocketException);

protected:
  WsaStartup m_wsaStartup;
  unsigned short m_port;
  struct in_addr m_addr;
  static LocalMutex s_resolveMutex;
};

