#pragma once
#include "winSystem/ParentProcess.h"
#include "ParentEventsListener.h"
#include "winSystem/WindowsEvent.h"
#include "Utils/CommonHeader.h"
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

