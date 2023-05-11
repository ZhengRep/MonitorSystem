#include "ZombieKiller.h"

ZombieKiller::ZombieKiller()
{
  resume();
}

ZombieKiller::~ZombieKiller()
{
  if (isActive()) {
    terminate();
    wait();
  }
  killAllZombies();
}

void ZombieKiller::execute()
{
  while (!isTerminating()) {
    deleteDeadZombies();
    Sleep(50);
  }
}

void ZombieKiller::addZombie(Thread* zombie)
{
  AutoLock l(&m_lockObj);

  m_zombies.push_back(zombie);
}

void ZombieKiller::deleteDeadZombies()
{
  AutoLock l(&m_lockObj);

  ThreadList::iterator iter = m_zombies.begin();
  while (iter != m_zombies.end()) {
    Thread* thread = *iter;
    if (!thread->isActive()) {
      delete thread;
      iter = m_zombies.erase(iter);
    } else {
      iter++;
    }
  }
}

void ZombieKiller::killAllZombies()
{
  AutoLock l(&m_lockObj);

  ThreadList::iterator iter;
  for (iter = m_zombies.begin(); iter != m_zombies.end(); iter++) {
    (*iter)->terminate();
  }
  for (iter = m_zombies.begin(); iter != m_zombies.end(); iter++) {
    (*iter)->wait();
  }

  deleteDeadZombies();
}

