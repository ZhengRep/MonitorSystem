#include "WindowsDisplays.h"
#include "thread/AutoLock.h"

WindowsDisplays::WindowsDisplays()
{
}

WindowsDisplays::~WindowsDisplays()
{
}

void WindowsDisplays::getDisplayCoordinates(unsigned char displayNumber, Rect* rect)
{
  AutoLock al(&m_displayRectsMutex);
  update();
  displayNumber--;
  if (displayNumber < m_displayRects.size()) {
    *rect = m_displayRects[displayNumber];
  } else {
    rect->clear();
  }
}

std::vector<Rect> WindowsDisplays::getDisplays()
{
  update();
  return m_displayRects;
}

void WindowsDisplays::update()
{
  if (!isAlreadyUpdated()) {
    m_displayRects.clear();
    m_xVirtualScreen = GetSystemMetrics(SM_XVIRTUALSCREEN);
    m_yVirtualScreen = GetSystemMetrics(SM_YVIRTUALSCREEN);

    // Enumerate only desktop's displays. Skip mirror driver desktops.
    HDC hdc = GetDC(0);
    EnumDisplayMonitors(hdc, 0, monitorEnumProc, (LPARAM)this);

    m_latestUpdateTime = DateTime::now();
    ReleaseDC(0, hdc);
  }
}

BOOL WindowsDisplays::monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
  WindowsDisplays* _this = (WindowsDisplays*)dwData;
  Rect rect(lprcMonitor->left - _this->m_xVirtualScreen,
    lprcMonitor->top - _this->m_yVirtualScreen,
    lprcMonitor->right - _this->m_xVirtualScreen,
    lprcMonitor->bottom - _this->m_yVirtualScreen);
  _this->m_displayRects.push_back(rect);
  return TRUE;
}

bool WindowsDisplays::isAlreadyUpdated()
{
  if ((DateTime::now() - m_latestUpdateTime).getTime() > UPDATE_INTERVAL) {
    return false;
  } else {
    return true;
  }
}
