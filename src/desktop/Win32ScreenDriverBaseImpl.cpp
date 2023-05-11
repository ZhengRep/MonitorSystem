#include "Win32ScreenDriverBaseImpl.h"

Win32ScreenDriverBaseImpl::Win32ScreenDriverBaseImpl(UpdateKeeper* updateKeeper,
  UpdateListener* updateListener, LocalMutex* fbLocalMutex, LogWriter* log)
  : WinVideoRegionUpdaterImpl(log),
  m_fbLocalMutex(fbLocalMutex),
  m_cursorPosDetector(updateKeeper, updateListener, log),
  m_curShapeDetector(updateKeeper, updateListener, &m_curShapeGrabber, fbLocalMutex, log)
{
}

Win32ScreenDriverBaseImpl::~Win32ScreenDriverBaseImpl()
{
  terminateDetection();
}

void Win32ScreenDriverBaseImpl::executeDetection()
{
  m_cursorPosDetector.resume();
  m_curShapeDetector.resume();
}

void Win32ScreenDriverBaseImpl::terminateDetection()
{
  m_cursorPosDetector.terminate();
  m_curShapeDetector.terminate();

  m_cursorPosDetector.wait();
  m_curShapeDetector.wait();
}

LocalMutex* Win32ScreenDriverBaseImpl::getFbMutex()
{
  return m_fbLocalMutex;
}

bool Win32ScreenDriverBaseImpl::grabCursorShape(const PixelFormat* pf)
{
  // Grabbing under the mutex avoid us from grab void cursor shape in time when the
  // shape hides until grabs screen.
  AutoLock al(m_fbLocalMutex);
  return m_curShapeGrabber.grab(pf);
}

const CursorShape* Win32ScreenDriverBaseImpl::getCursorShape()
{
  return m_curShapeGrabber.getCursorShape();
}

Point Win32ScreenDriverBaseImpl::getCursorPosition()
{
  AutoLock al(m_fbLocalMutex);
  return m_cursorPosDetector.getCursorPos();
}

void Win32ScreenDriverBaseImpl::getCopiedRegion(Rect* copyRect, Point* source)
{
  AutoLock al(m_fbLocalMutex);
  m_copyRectDetector.detectWindowMovements(copyRect, source);
}
