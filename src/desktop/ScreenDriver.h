#pragma once
#include "region/Dimension.h"
#include "region/Region.h"
#include "rfb/FrameBuffer.h"
#include "rfb/CursorShape.h"

class ScreenDriver
{
public:
  virtual ~ScreenDriver() {}
  virtual void executeDetectin() = 0;
  virtual void terminateDetectin() = 0;
  virtual Dimension getScreenDimension() = 0;
  virtual bool grabFb(const Rect* rect = 0) = 0;
  virtual FrameBuffer* getScreenBuffer() = 0;
  virtual bool getScreenPropertiesChanged() = 0;
  virtual bool getScreenSizeChanged() = 0;
  virtual bool applyNewScreenProperties() = 0;
  virtual bool grabCursorShape(const PixelFormat* pf) = 0;
  virtual const CursorShape* getCursorShape() = 0;
  virtual Point getCursorPosition() = 0;
  virtual void getCopiedRegion(Rect* copyRect, Point* source) = 0;
  virtual Region getVideoRegion() = 0;
};

