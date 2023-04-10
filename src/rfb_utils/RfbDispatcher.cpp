#include "RfbDispatcher.h"

RfbDispatcher::RfbDispatcher(RfbInputGate* gate, AnEventListener* extTerminationListener)
  :m_gate(gate),
  m_extTerminationListener(extTerminationListener),
  m_terminationEvent(0)
{
}

RfbDispatcher::RfbDispatcher(RfbInputGate* gate, WindowsEvent* terminationEvent)
  : m_gate(gate),
  m_extTerminationListener(0),
  m_terminationEvent(terminationEvent)
{
}

RfbDispatcher::~RfbDispatcher()
{
  terminate();
  wait();
}

void RfbDispatcher::registerNewHandle(UINT32 code, RfbDispatcherListener* listener)
{
  m_handlers[code] = listener;
}

void RfbDispatcher::execute()
{
  try {
    while (!isTerminating()) {
      UINT32 code = m_gate->readInt8();
      if (code == 0xfc) { //special vnc code
        code = code << 24;
        code += m_gate->readUInt8() << 16;
        code += m_gate->readUInt8() << 8;
        code += m_gate->readUInt8();
      }
      std::map<UINT32, RfbDispatcherListener*>::iterator it = m_handlers.find(code);
      if (it == m_handlers.end()) {
        StringStorage errMsg;
        errMsg.format(_T("Unhandled %d code has been received from client"), code);
        throw Exception(errMsg.getString());
      }
      (*it).second->onRequest(code, m_gate);
    }
  }
  catch(...){}
  notifyAbTermination();

}

void RfbDispatcher::notifyAbTermination()
{
  if (m_extTerminationListener) {
    m_extTerminationListener->onAnObjectEvent();
  }
  if (m_terminationEvent) {
    m_terminationEvent->notify();
  }
}
