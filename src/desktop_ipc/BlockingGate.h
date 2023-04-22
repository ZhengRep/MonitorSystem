#pragma once
#include "thread/LocalMutex.h"
#include "io_lib/Channel.h"
#include "io_lib/DataInputStream.h"
#include "io_lib/DataOutputStream.h"

class BlockingGate: public LocalMutex, public DataOutputStream, public DataInputStream
{
public:
  BlockingGate(Channel* stream);
  virtual ~BlockingGate(){};
};