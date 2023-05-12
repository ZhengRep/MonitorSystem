#pragma once
#include "thread/GuiThread.h"
#include "utils/AnEventListener.h"
#include "log_writer/LogWriter.h"

class SessionChangesWatcher : public GuiThread
{
public:
  SessionChangesWatcher(AnEventListener* extSessionChangesListener, LogWriter* log);
  virtual ~SessionChangesWatcher();

protected:
  virtual void execute();

  DWORD m_baseSessionId;
  AnEventListener* m_extSessionChangesListener;

  LogWriter* m_log;
};

