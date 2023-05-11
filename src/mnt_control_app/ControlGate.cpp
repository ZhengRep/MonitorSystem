#include "ControlGate.h"

ControlGate::ControlGate(Channel* stream)
  : DataInputStream(stream), DataOutputStream(stream)
{
}

ControlGate::~ControlGate()
{
}

void ControlGate::skipBytes(UINT32 length)
{
  char one;
  for (size_t i = 0; i < length; i++) {
    readFully(&one, 1);
  }
}
