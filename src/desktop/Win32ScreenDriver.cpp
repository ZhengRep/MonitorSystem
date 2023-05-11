#include "Win32ScreenDriver.h"
#include "Utils/Macros.h"

Win32ScreenDriver::Win32ScreenDriver(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
  FrameBuffer* fb, LocalMutex* fbLocalMutex, LogWriter* log)
  : Win32ScreenDriverBaseImpl(updateKeeper, updateListener, fbLocalMutex, log),
  m_poller(updateKeeper, updateListener, &m_screenGrabber, fb, fbLocalMutex, log),
  m_consolePoller(updateKeeper, updateListener, &m_screenGrabber, fb, fbLocalMutex, log),
  m_hooks(updateKeeper, updateListener, log)
{
}

Win32ScreenDriver::~Win32ScreenDriver()
{
  terminateDetection();
}

void Win32ScreenDriver::executeDetection()
{
  Win32ScreenDriverBaseImpl::executeDetection();
  m_poller.resume();
  m_consolePoller.resume();
  m_hooks.resume();
}

void Win32ScreenDriver::terminateDetection()
{
  m_poller.terminate();
  m_consolePoller.terminate();
  m_hooks.terminate();

  Win32ScreenDriverBaseImpl::terminateDetection();

  m_poller.wait();
  m_consolePoller.wait();
  m_hooks.wait();
}

Dimension Win32ScreenDriver::getScreenDimension()
{
  AutoLock al(getFbMutex());
  return Dimension(&unmove(m_screenGrabber.getScreenRect()));
}

bool Win32ScreenDriver::grabFb(const Rect* rect)
{
  AutoLock al(getFbMutex());
  return m_screenGrabber.grab(rect);
}

FrameBuffer* Win32ScreenDriver::getScreenBuffer()
{
  return m_screenGrabber.getScreenBuffer();
}

bool Win32ScreenDriver::getScreenPropertiesChanged()
{
  AutoLock al(getFbMutex());
  return m_screenGrabber.getPropertiesChanged();
}

bool Win32ScreenDriver::getScreenSizeChanged()
{
  AutoLock al(getFbMutex());
  return m_screenGrabber.getScreenSizeChanged();
}

bool Win32ScreenDriver::applyNewScreenProperties()
{
  AutoLock al(getFbMutex());
  return m_screenGrabber.applyNewProperties();
}
