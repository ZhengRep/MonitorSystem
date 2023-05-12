#pragma once
#include "UpdateDetector.h"
#include "ScreenGrabber.h"
#include "win_system/WindowsEvent.h"
#include "log_writer/LogWriter.h"

class ConsolePoller : public UpdateDetector
{
public:
  ConsolePoller(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
    ScreenGrabber* screenGrabber, FrameBuffer* backupFrameBuffer,
    LocalMutex* frameBufferMutex, LogWriter* log);

  virtual ~ConsolePoller();

protected:
  virtual void execute();
  virtual void onTerminate();

private:
  Rect getConsoleRect();

  ScreenGrabber* m_screenGrabber;
  FrameBuffer* m_backupFrameBuffer;
  LocalMutex* m_frameBufferMutex;
  Rect m_pollingRect;
  WindowsEvent m_intervalWaiter;
  LogWriter* m_log;
};

