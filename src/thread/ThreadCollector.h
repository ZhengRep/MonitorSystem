#pragma once
#include "Thread.h"
#include "LocalMutex.h"
#include "winSystem/WindowsEvent.h"
#include <list>

using namespace std;

typedef list<Thread*> ThreadList;

class ThreadCollector : protected Thread
{
public:
  ThreadCollector();
  virtual ~ThreadCollector();
  virtual void addThread(Thread* thread);
  void destroyAllThreads();
protected:
  virtual void execute();
  void deleteDeadThreads();

protected:
  ThreadList m_threads;
  LocalMutex m_lockObj;

  WindowsEvent m_timer;
};

