#pragma once
#include "rfb/CursorShape.h"

class CursorShapeGrabber
{
public:
  CursorShapeGrabber();
  virtual ~CursorShapeGrabber();

  virtual bool grab(const PixelFormat* pixelFormat) = 0;
  virtual bool isCursorShapeChanged() = 0;

  virtual const CursorShape* getCursorShape() const;

protected:
  CursorShape m_cursorShape;
};

