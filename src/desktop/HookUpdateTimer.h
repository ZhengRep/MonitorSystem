#pragma once
#include "thread/Thread.h"
#include "UpdateListener.h"
#include "winSystem/WindowsEvent.h"

class HookUpdateTimer : public Thread
{
public:
  HookUpdateTimer(UpdateListener* updateListener);
  virtual ~HookUpdateTimer();

  void sear();

protected:
  virtual void execute();
  virtual void onTerminate();

  WindowsEvent m_updateWaiter;
  WindowsEvent m_timer;
  UpdateListener* m_updateListener;
};

