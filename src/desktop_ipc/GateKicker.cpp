#include "GateKicker.h"
#include "thread/AutoLock.h"

GateKicker::GateKicker(BlockingGate* gate)
  :m_gate(gate)
{
  resume();
}

GateKicker::~GateKicker()
{
  terminate();
  wait();
}

void GateKicker::execute()
{
  while (!isTerminating()) {
    m_sleeper.waitForEvent(500);
    if (!isTerminating()) {
      try {
        AutoLock al(m_gate);
        m_gate->writeUInt8(255);
      } catch (...) {

      }
    }
  }
}

void GateKicker::onTerminate()
{
  m_sleeper.notify();
}
