#include "ThreadCollector.h"
#include "thread/AutoLock.h"

ThreadCollector::ThreadCollector()
{
  resume();
}

ThreadCollector::~ThreadCollector()
{
  if (isActive()) {
    terminate();
    m_timer.notify();
    wait();
  }
  destroyAllThreads();
}

void ThreadCollector::execute()
{
  while (!isTerminating()) {
    deleteDeadThreads();
    m_timer.waitForEvent(50);
  }
}

void ThreadCollector::addThread(Thread* thread)
{
  AutoLock l(&m_lockObj);
  m_threads.push_back(thread);
}

void ThreadCollector::deleteDeadThreads()
{
  AutoLock l(&m_lockObj);

  ThreadList::iterator iter = m_threads.begin();
  while (iter != m_threads.end()) {
    Thread* thread = *iter;
    if (!thread->isActive()) {
      delete thread;
      iter = m_threads.erase(iter);
    } else {
      iter++;
    }
  }
}

void ThreadCollector::destroyAllThreads()
{
  AutoLock l(&m_lockObj);

  ThreadList::iterator iter;
  for (iter = m_threads.begin(); iter != m_threads.end(); iter++) {
    (*iter)->terminate();
  }
  for (iter = m_threads.begin(); iter != m_threads.end(); iter++) {
    (*iter)->wait();
  }

  deleteDeadThreads();
}
