#pragma once
#include "io_lib/Channel.h"
#include "SocketAddressIPv4.h"
#include "SocketIPv4.h"
#include "io_lib/IOException.h"

class SocketStream: public Channel
{
public:
  SocketStream(SocketIPv4*);
  virtual ~SocketStream();

  virtual size_t read(void*, size_t) throw(IOException);

  virtual size_t write(const void*, size_t) throw(IOException);
  virtual void close();

  virtual size_t available();

protected:
  SocketIPv4* m_socket;
  friend class SocketIPv4;
};

