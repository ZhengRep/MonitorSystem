#pragma once
#include "CursorShapeGrabber.h"
#include "Utils/CommonHeader.h"
#include "winSystem/Screen.h"

class WindowsCursorShapeGrabber : public CursorShapeGrabber
{
public:
  WindowsCursorShapeGrabber(void);
  virtual ~WindowsCursorShapeGrabber(void);

  virtual bool grab(const PixelFormat* pixelFormat);

  virtual bool isCursorShapeChanged();

private:
  bool grabPixels(const PixelFormat* pixelFormat);

  HCURSOR getHCursor();

  HCURSOR m_lastHCursor;
  Screen m_screen;
};

