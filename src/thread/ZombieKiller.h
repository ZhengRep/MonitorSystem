#pragma once
#include "Thread.h"
#include "LocalMutex.h"
#include "utils/Singleton.h"

#include <list>
using namespace std;

typedef list<Thread*> ThreadList;

class ZombieKiller: private Thread, public Singleton<ZombieKiller>
{
public:
  ZombieKiller();
  virtual ~ZombieKiller();
  void addZombie(Thread* zombie);
  void killAllZombies();

protected:
  virtual void execute();
  void deleteDeadZombies();

  ThreadList m_zombies;
  LocalMutex m_lockObj;
};

