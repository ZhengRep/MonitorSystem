#include "CursorShapeGrabber.h"

CursorShapeGrabber::CursorShapeGrabber()
{
}

CursorShapeGrabber::~CursorShapeGrabber()
{
}

const CursorShape* CursorShapeGrabber::getCursorShape() const
{
  return &m_cursorShape;
}
