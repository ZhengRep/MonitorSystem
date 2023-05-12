#pragma once
#include "win_system/ParentProcess.h"
#include "ParentEventsListener.h"
#include "win_system/WindowsEvent.h"
#include "utils/CommonHeader.h"
#include "thread/Thread.h"
#include <TlHelp32.h>

class ParentWatcher: private Thread
{
public:
  ParentWatcher(ParentEventsListener* parentEventListener);
  virtual ~ParentWatcher();

private:
  virtual void execute();
  virtual void onTerminate();

  ParentEventsListener* m_parentEventListener;
  ParentProcess m_parentProcess;
  WindowsEvent m_timer;
};

