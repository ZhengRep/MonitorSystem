#pragma once
#include "thread/Thread.h"
#include "win_system/WindowsEvent.h"
#include "TimerListener.h"

class ConnectionTimer : private Thread
{
public:
  ConnectionTimer(TimerListener* timerListener, unsigned int timeOut);
  virtual ~ConnectionTimer();

private:
  virtual void execute();
  virtual void onTerminate();

  TimerListener* m_timerListener;
  unsigned int m_timeOut;
  WindowsEvent m_timer;
  bool m_signalEnabled;
};

