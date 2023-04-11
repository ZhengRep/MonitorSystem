#include "ViewPort.h"
#include "thread/AutoLock.h"
#include <crtdbg.h>
#include "Utils/BrokenHandleException.h"
#include "Utils/Macros.h"

ViewPort::ViewPort(LogWriter* log)
  :m_desktop(0),
  m_log(log)
{
}

ViewPort::ViewPort(const ViewPortState* viewPortState, LogWriter* log)
  :m_desktop(0),
  m_state(*viewPortState),
  m_log(log)
{
}

ViewPort::~ViewPort()
{
}

void ViewPort::initDesktopInterface(Desktop* desktop)
{
  m_desktop = desktop;
}

void ViewPort::update(const Dimension* fbDimension)
{
  AutoLock al(&m_stateMutex);

  Rect rect;
  switch (m_state.m_mode) {
  case ViewPortState::APPLICATION:
    _ASSERT(m_desktop != 0);
    m_desktop->getApplicationRegion(m_state.m_processId, &m_appRegion);

  case ViewPortState::FULL_DESKTOP:
    rect.setRect(&unmove(fbDimension->getRect()));
    break;

  case ViewPortState::PRIMARY_DISPLAY:
    _ASSERT(m_desktop != 0);
    m_desktop->getPrimaryDesktopCoords(&rect);
    break;

  case ViewPortState::DISPLAY_NUMBER:
    _ASSERT(m_desktop != 0);
    if (!m_state.m_windowIsResolved) {
      resolveWindowName();
    }
    if (m_state.m_windowIsResolved) {
      try {
        m_desktop->getWindowCoords(m_state.m_hwnd, &rect);
      } catch (BrokenHandleException& e) {
        m_log->error(_T("ViewPort::update: %s"), e.getMessage());
        m_state.unresolveHwnd();
      }
    }
    break;
  case ViewPortState::ARBITRARY_RECT:
    rect = m_state.m_arbitraryRect;
    _ASSERT(m_desktop != 0);
    m_desktop->getNormalizedRect(&rect);
    break;
  }

  m_log->debug(_T("View port coordinates: (%d, %d, %dx%d)"), rect.left, rect.top, rect.getWidth(), rect.getHeight());
  m_rect = rect.intersection(&unmove(fbDimension->getRect()));
  if (m_rect.getWidth() < 0 || m_rect.getHeight() < 0) {
    m_rect.clear();
  }

  m_log->debug(_T("Constrained (to the FrameBuffer dimension) view port coordinates: (%d, %d %dx%d)"), rect.left, rect.top, rect.getWidth(), rect.getHeight());
}

Rect ViewPort::getViewPortRect()
{
  AutoLock al(&m_stateMutex);
  return m_rect;
}

bool ViewPort::getOnlyApplication()
{
  return m_state.m_mode == ViewPortState::APPLICATION;
}

unsigned int ViewPort::getApplicationId()
{
  return m_state.m_processId;
}

void ViewPort::getApplicationRegion(Region* region)
{
  *region = m_appRegion;
}

void ViewPort::changedState(const ViewPortState* newState)
{
  AutoLock al(&m_stateMutex);
  m_state = *newState;
}

void ViewPort::resolveWindowName()
{
  if ((DateTime::now() - m_latestHwndResolveingTime).getTime() > RESOLVING_PERIOD) {
    HWND hwnd = m_desktop->getWindowHandleByName(&m_state.m_windowName);
    if (hwnd != 0) {
      m_state.setWindowHandle(hwnd);
    }
    m_latestHwndResolveingTime = DateTime::now();
  }
}
