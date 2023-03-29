#include "RfbOutputGate.h"

RfbOutputGate::RfbOutputGate(OutputStream* stream)
  :DataOutputStream(0)
{
  m_tunnel = new BufferedOutputStream(stream);
}

RfbOutputGate::~RfbOutputGate()
{
  delete m_tunnel;
}

void RfbOutputGate::flush() throw(IOException)
{
  m_tunnel->flush();
}
