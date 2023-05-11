#pragma once
#include "ScreenDriver.h"
#include "Win32ScreenDriverBaseImpl.h"
#include "Poller.h"
#include "ConsolePoller.h"
#include "WindowsScreenGrabber.h"
#include "HooksUpdateDetector.h"

class Win32ScreenDriver : public Win32ScreenDriverBaseImpl
{
public:
  Win32ScreenDriver(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
    FrameBuffer* fb, LocalMutex* fbLocalMutex, LogWriter* log);
  virtual ~Win32ScreenDriver();

  virtual void executeDetection();
  virtual void terminateDetection();

  virtual Dimension getScreenDimension();
  virtual bool grabFb(const Rect* rect = 0);
  virtual FrameBuffer* getScreenBuffer();
  virtual bool getScreenPropertiesChanged();
  virtual bool getScreenSizeChanged();
  virtual bool applyNewScreenProperties();

private:
  WindowsScreenGrabber m_screenGrabber;
  Poller m_poller;
  ConsolePoller m_consolePoller;
  HooksUpdateDetector m_hooks;
};

