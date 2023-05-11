#pragma once
#include "Transport.h"
#include "network/socket/SocketIPv4.h"

class SocketIPv4Transport : public Transport
{
public:
  SocketIPv4Transport(SocketIPv4* socket);
  virtual ~SocketIPv4Transport();
  virtual Channel* getIOStream();
  virtual Transport* accept() throw(SocketException);

  virtual void close() throw(SocketException);

private:

  SocketIPv4* m_socket;

  Channel* m_stream;
};

