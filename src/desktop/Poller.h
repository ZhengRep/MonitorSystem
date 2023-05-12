#pragma once
#include "UpdateDetector.h"
#include "ScreenGrabber.h"
#include "rfb/FrameBuffer.h"
#include "region/Rect.h"
#include "win_system/WindowsEvent.h"
#include "log_writer/LogWriter.h"

#define DEFAULT_SLEEP_TIME 1000

class Poller : public UpdateDetector
{
public:
  Poller(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
    ScreenGrabber* screenGrabber, FrameBuffer* backupFrameBuffer,
    LocalMutex* frameBufferCriticalSection, LogWriter* log);

  virtual ~Poller();

protected:
  virtual void execute();
  virtual void onTerminate();

private:
  ScreenGrabber* m_screenGrabber;
  FrameBuffer* m_backupFrameBuffer;
  LocalMutex* m_fbMutex;
  Rect m_pollingRect;
  WindowsEvent m_intervalWaiter;

  LogWriter* m_log;
};

