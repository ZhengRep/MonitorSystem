#pragma once
#include "NewConnectionEvents.h"
#include "win_event_log/WinEventLog.h"
#include "win_system/ApplicationCrashEvents.h"
#include "WinServiceEvents.h"

class WinEventLogWriter :public NewConnectionEvents,
  public ApplicationCrashEvents,
  public WinServiceEvents
{
public:
  WinEventLogWriter(LogWriter* log);
  ~WinEventLogWriter();

  virtual void enable();

  // The NewConnectionEvents implementations.
  virtual void onSuccAuth(const StringStorage* ip);
  virtual void onAuthFailed(const StringStorage* ip);
  virtual void onDisconnect(const StringStorage* message);

  // The ApplicationCrashEvents implementations.
  virtual void onCrash(const StringStorage* dumpPath);

  // The WinServiceEvents implementations.
  virtual void onSuccServiceStart();
  virtual void onFailedServiceStart(const StringStorage* reason);
  virtual void onServiceStop();

private:
  WinEventLog m_sysLog;
};

