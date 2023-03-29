#pragma once
#include "io_lib/Channel.h"
#include "io_lib/DataInputStream.h"

class RfbInputGate: public DataInputStream
{
public:
  RfbInputGate(Channel* stream);
  RfbInputGate(InputStream* stream);
  virtual ~RfbInputGate();
};

