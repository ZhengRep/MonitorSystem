#include "ScreenGrabber.h"

ScreenGrabber::ScreenGrabber(void)
{
}

ScreenGrabber::~ScreenGrabber(void)
{
}

bool ScreenGrabber::grab(const Rect* rect)
{
    return false;
}

void ScreenGrabber::setWorkRect(Rect* workRect)
{
  m_offsetFrameBuffer.x = workRect->left;
  m_offsetFrameBuffer.y = workRect->top;
  m_workFrameBuffer.setEmptyDimension(workRect);
}

bool ScreenGrabber::applyNewProperties()
{
  if (!applyNewPixelFormat() || !applyNewFullScreenRect()) {
    return false;
  }

  return true;
}

bool ScreenGrabber::setWorkRectDefault()
{
  // Set workRect to full screen by default
  if (!applyNewFullScreenRect()) {
    return false;
  }

  setWorkRect(&m_fullScreenRect);

  return true;
}
