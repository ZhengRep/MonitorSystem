#pragma once
#include "WindowsScreenGrabber.h"
#include "ScreenDriver.h"
#include "rfb/FrameBuffer.h"
#include "thread/LocalMutex.h"
#include "UpdateContainer.h"
#include "GrabOptimizator.h"

class UpdateFilter
{
public:
  UpdateFilter(ScreenDriver* screenDriver, FrameBuffer* frameBuffer, LocalMutex* frameBufferCriticalSection, LogWriter* log);
  ~UpdateFilter();
  void filter(UpdateContainer* updateContainer);

private:
  void getChangedRegion(Region* rgn, const Rect* rect);
  void updateChangedRect(Region* rgn, const Rect* rect);
  void updateChangedSubRect(Region* rgn, const Rect* rect);

  ScreenDriver* m_screenDriver;
  FrameBuffer* m_frameBuffer;
  LocalMutex* m_fbMutex;
  GrabOptimizator m_grabOptimizator;

  LogWriter* m_log;
};

