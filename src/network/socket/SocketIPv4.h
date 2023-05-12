#pragma once
#include "SocketAddressIPv4.h"
#include "SocketException.h"
#include "io_lib/Channel.h"
#include "io_lib/IOException.h"
#include "win_system/WsaStartup.h"
#include "thread/LocalMutex.h"

class SocketIPv4
{
public:
  SocketIPv4();
  virtual ~SocketIPv4();
  void connect(const TCHAR* host, unsigned short port) throw(SocketException);
  void connect(const SocketAddressIPv4& addr) throw(SocketException);
  void close() throw(SocketException);
  void shutdown(int how) throw(SocketException);
  void bind(const TCHAR* bindHost, unsigned short bindPort);
  void bind(const SocketAddressIPv4& addr) throw(SocketException);
  bool isBound();
  void listen(int backlog) throw(SocketException);
  SocketIPv4* accept() throw (SocketException);
  int send(const char* data, int size, int flags = 0) throw(IOException);
  int recv(char* buffer, int size, int flags = 0) throw(IOException);
  int available();
  bool getLocalAddr(SocketAddressIPv4* addr);
  bool getPeerAddr(SocketAddressIPv4* addr);
  /* Auxiliary */
  void setSocketOptions(int level, int name, void* value, socklen_t len) throw(SocketException);
  void getSocketOptions(int level, int name, void* value, socklen_t* len) throw(SocketException);

  /* Socket options */
  void enableNaggleAlgorithm(bool enabled) throw(SocketException);
  void setExclusiveAddrUse() throw(SocketException);

private:
  WsaStartup m_wsaStartup;

protected:
  SOCKET getAcceptedSocket(sockaddr_in* addr);
  void set(SOCKET socket);

  LocalMutex m_mutex;
  SOCKET m_socket;
  bool m_isClosed;

  SocketAddressIPv4* m_localAddr;
  SocketAddressIPv4* m_peerAddr;

  /**
   * Flag determinating if socket is server or client socket.
   */
  bool m_isBound;
};

