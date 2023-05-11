#include "ConnectionTimer.h"

ConnectionTimer::ConnectionTimer(TimerListener* timerListener,
  unsigned int timeOut)
  : m_timerListener(timerListener),
  m_timeOut(timeOut),
  m_signalEnabled(true)
{
  resume();
}

ConnectionTimer::~ConnectionTimer()
{
  m_signalEnabled = false;
  terminate();
  wait();
}

void ConnectionTimer::onTerminate()
{
  m_timer.notify();
}

void ConnectionTimer::execute()
{
  m_timer.waitForEvent(m_timeOut);
  if (m_signalEnabled) {
    m_timerListener->onTimeTimer();
  }
}
