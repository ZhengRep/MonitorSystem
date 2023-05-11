#include "ParentWatcher.h"

ParentWatcher::ParentWatcher(ParentEventsListener* parentEventListener)
  :m_parentEventListener(parentEventListener)
{
}

ParentWatcher::~ParentWatcher()
{
  terminate();
  wait();
}

void ParentWatcher::execute()
{
  while (!isTerminating()) {
    if (!m_parentProcess.isAlive()) {
      m_parentEventListener->onParentTerminate();
    }
    m_timer.waitForEvent(1000);
  }
}

void ParentWatcher::onTerminate()
{
  m_timer.notify();
}
