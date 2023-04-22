#include "BlockingGate.h"

BlockingGate::BlockingGate(Channel* stream)
  :DataInputStream(stream), DataOutputStream(stream)
{
}
