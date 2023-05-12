#pragma once
#include "thread/Thread.h"
#include "log_writer/LogWriter.h"

// This class runs MntControl in current session.
// This class only for application mode running.
class WsConfigRunner : private Thread
{
public:
  WsConfigRunner(Logger* logger, bool serviceMode = false);
  virtual ~WsConfigRunner();

protected:
  virtual void execute();

private:
  bool m_serviceMode;

  LogWriter m_log;
};

