#pragma once
#include "BlockingGate.h"
#include "thread/Thread.h"
#include "win_system/WindowsEvent.h"

class GateKicker: public Thread
{
public:
  GateKicker(BlockingGate* gate);
  virtual ~GateKicker();

protected:
  virtual void execute();
  virtual void onTerminate();
private:
  BlockingGate* m_gate;
  WindowsEvent m_sleeper;
};

