#pragma once
#include "UpdateKeeper.h"
#include "CursorShapeGrabber.h"
#include "UpdateDetector.h"
#include "win_system/WindowsEvent.h"
#include "log_writer/LogWriter.h"

class CursorShapeDetector : public UpdateDetector
{
public:
  CursorShapeDetector(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
    CursorShapeGrabber* mouseGrabber, LocalMutex* mouseGrabLocMut, LogWriter* log);
  virtual ~CursorShapeDetector();

protected:
  virtual void execute();
  virtual void onTerminate();

  CursorShapeGrabber* m_mouseGrabber;
  LocalMutex* m_mouseGrabLocMut;
  WindowsEvent m_sleepTimer;

  LogWriter* m_log;
};

