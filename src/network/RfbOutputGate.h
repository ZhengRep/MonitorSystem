#pragma once
#include "io_lib/DataOutputStream.h"
#include "io_lib/BufferedOutputStream.h"
#include "thread/LocalMutex.h"

class RfbOutputGate: public DataOutputStream, public LocalMutex
{
public:
  RfbOutputGate(OutputStream* stream);
  virtual ~RfbOutputGate();

  virtual void flush() throw(IOException);

private:
  BufferedOutputStream* m_tunnel;

};

