#pragma once
#include "ScreenDriver.h"
#include "CursorPositionDetector.h"
#include "CursorShapeDetector.h"
#include "CopyRectDetector.h"
#include "WindowsCursorShapeGrabber.h"
#include "WinVideoRegionUpdaterImpl.h"

class Win32ScreenDriverBaseImpl: public WinVideoRegionUpdaterImpl
{
public:
  Win32ScreenDriverBaseImpl(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
    LocalMutex* fbLocalMutex, LogWriter* log);
  virtual ~Win32ScreenDriverBaseImpl();

  virtual void executeDetection();
  virtual void terminateDetection();

  virtual bool grabCursorShape(const PixelFormat* pf);
  virtual const CursorShape* getCursorShape();
  virtual Point getCursorPosition();

  virtual void getCopiedRegion(Rect* copyRect, Point* source);

protected:
  LocalMutex* getFbMutex();

private:
  LocalMutex* m_fbLocalMutex;

  CursorPositionDetector m_cursorPosDetector;
  WindowsCursorShapeGrabber m_curShapeGrabber;
  CursorShapeDetector m_curShapeDetector;

  CopyRectDetector m_copyRectDetector;
};

