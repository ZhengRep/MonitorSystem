#pragma once
#include "thread/Thread.h"
#include "win_system/Process.h"
#include "log_writer/LogWriter.h"
#include "desktop_ipc/ReconnectionListener.h"

class DesktopServerWatcher: public Thread
{
public:
  DesktopServerWatcher(ReconnectionListener* recListener, LogWriter* log);
  virtual ~DesktopServerWatcher();

protected:
  virtual void execute();
  virtual void onTerminate();
  void start();
  void doXPTrick();

  Process* m_process;
  ReconnectionListener* m_recListener;
  LogWriter* m_log;
};

