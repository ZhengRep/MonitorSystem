#include "ViewPortState.h"

ViewPortState::ViewPortState()
  :m_mode(FULL_DESKTOP),
  m_hwnd(0),
  m_windowIsResolved(false),
  m_displayNumber(1),
  m_processId(0)
{
}

ViewPortState::ViewPortState(const ViewPortState& viewPortState)
{
  *this = viewPortState;
}

ViewPortState& ViewPortState::operator=(const ViewPortState& viewPortState)
{
  m_mode = viewPortState.m_mode;
  m_arbitraryRect = viewPortState.m_arbitraryRect;
  m_hwnd = viewPortState.m_hwnd;
  m_windowName = viewPortState.m_windowName;
  m_windowIsResolved = viewPortState.m_windowIsResolved;
  m_displayNumber = viewPortState.m_displayNumber;
  m_processId = viewPortState.m_processId;

  return *this;
}

ViewPortState::~ViewPortState()
{
}

void ViewPortState::setFullDesktop()
{
  m_mode = FULL_DESKTOP;
}

void ViewPortState::setArbitraryRect(const Rect* rect)
{
  m_mode = ARBITRARY_RECT;
  m_arbitraryRect = *rect;
}

void ViewPortState::setPrimaryDisplay()
{
  m_mode = PRIMARY_DISPLAY;
}

void ViewPortState::setDisplayNumber(unsigned char displayNumber)
{
  m_mode = DISPLAY_NUMBER;
  m_displayNumber = displayNumber;
}

void ViewPortState::setWindowHandle(HWND hwnd)
{
  m_mode = WINDOW_RECT;
  m_hwnd = hwnd;
  m_windowIsResolved = true;
}

void ViewPortState::setWindowName(const StringStorage* windowName)
{
  m_mode = WINDOW_RECT;
  m_windowName = *windowName;
  m_windowIsResolved = false;
}

void ViewPortState::unresolveHwnd()
{
  m_windowIsResolved = false;
  m_hwnd = 0;
}

void ViewPortState::setProcessId(unsigned int processId)
{
  m_mode = APPLICATION;
  m_processId = processId;
}
