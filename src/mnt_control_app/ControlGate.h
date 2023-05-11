#pragma once
#include "io_lib/DataInputStream.h"
#include "io_lib/DataOutputStream.h"
#include "io_lib/Channel.h"
#include "thread/LocalMutex.h"

class ControlGate : public DataInputStream, public DataOutputStream,
  public LocalMutex
{
public:
  ControlGate(Channel* stream);
  virtual ~ControlGate();

  void skipBytes(UINT32 length) throw(IOException);
};

