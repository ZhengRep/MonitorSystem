#pragma once
#include "UpdateDetector.h"
#include "win_system/WindowsEvent.h"
#include "log_writer/LogWriter.h"
#include "win_system/WinCursor.h"

class CursorPositionDetector : public UpdateDetector
{
public:
  CursorPositionDetector(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
    LogWriter* log);
  virtual ~CursorPositionDetector(void);

  Point getCursorPos();

protected:
  virtual void execute();
  virtual void onTerminate();

private:
  WinCursor m_cursor;
  WindowsEvent m_sleepTimer;
  Point m_lastCursorPos;
  LogWriter* m_log;
};

