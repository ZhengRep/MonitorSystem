#include "HookUpdateTimer.h"

HookUpdateTimer::HookUpdateTimer(UpdateListener* updateListener)
  : m_updateListener(updateListener)
{
  resume();
}

HookUpdateTimer::~HookUpdateTimer()
{
  terminate();
  wait();
}

void HookUpdateTimer::onTerminate()
{
  m_updateWaiter.notify();
  m_timer.notify();
}

void HookUpdateTimer::execute()
{
  while (!isTerminating()) {
    m_updateWaiter.waitForEvent();
    m_timer.waitForEvent(100);
    m_updateListener->onUpdate();
  }
}

void HookUpdateTimer::sear()
{
  m_updateWaiter.notify();
}
