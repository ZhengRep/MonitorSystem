#include "ViewPort.h"
#include "thread/AutoLock.h"

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
}

Rect ViewPort::getViewPortRect()
{
  AutoLock al(&m_stateMutex);
  return m_rect;
}

bool ViewPort::getOnlyApplication()
{
    return false;
}

unsigned int ViewPort::getApplicationId()
{
    return 0;
}

void ViewPort::getApplicationRegion(Region* region)
{
}

void ViewPort::changedState(const ViewPortState* newState)
{
}

ViewPort::ViewPort(const ViewPort&)
{
}

ViewPort& ViewPort::operator=(const ViewPort&)
{
    // TODO: insert return statement here
}

void ViewPort::resolveWindowName()
{
}
