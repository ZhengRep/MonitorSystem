#pragma once
#include "win_system/Process.h"
#include "log_writer/LogWriter.h"

class CurrentConsoleProcess:public Process
{
public:
  CurrentConsoleProcess(LogWriter* log, bool connectRdpSession, const TCHAR* path = 0, const TCHAR* args = 0);
  virtual ~CurrentConsoleProcess();
  virtual void start() throw(SystemException);

private:
  LogWriter* m_log;
  bool m_connectRdpSession;
};

