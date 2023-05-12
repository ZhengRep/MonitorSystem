#pragma once
#include "thread/Thread.h"
#include "RfbDispatcherListener.h"
#include "utils/AnEventListener.h"
#include "win_system/WindowsEvent.h"
#include<map>

class RfbDispatcher: public Thread
{
public:
  RfbDispatcher(RfbInputGate* gate, AnEventListener* extTerminationListener);
  RfbDispatcher(RfbInputGate* gate, WindowsEvent* terminationEvent);
  virtual ~RfbDispatcher();

  void registerNewHandle(UINT32 code, RfbDispatcherListener* listener);

protected:
  virtual void execute();
  void notifyAbTermination();
  RfbInputGate* m_gate;
  std::map<UINT32, RfbDispatcherListener*> m_handlers;
  AnEventListener* m_extTerminationListener;
  WindowsEvent* m_terminationEvent;
};

