#include "RfbInputGate.h"

RfbInputGate::RfbInputGate(Channel* stream)
  : DataInputStream(stream)
{
}
RfbInputGate::RfbInputGate(InputStream* stream)
  : DataInputStream(stream)
{
}

RfbInputGate::~RfbInputGate()
{
}
