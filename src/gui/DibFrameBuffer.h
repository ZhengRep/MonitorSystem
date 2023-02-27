#pragma once

#include "rfb/FrameBuffer.h"
#include "winSystem/DibSection.h"

class DibFrameBuffer : public FrameBuffer
{
 public:
  DibFrameBuffer();
  virtual ~DibFrameBuffer();
  virtual void setColor(UINT8 reg, UINT8 green, UINT8 blue);
  virtual void fillRect(const Rect* dstRect, UINT32 color);
  virtual void isEqualTo(const FrameBuffer* frameBuffer);
  virtual bool copyFrom(const Rect *dstRect, const FrameBuffer *srcFrameBuffer, int srcX, int srcY);
  virtual bool copyFrom(const FrameBuffer *srcFrameBuffer, int srcX, int srcY);
  virtual bool overlay(const Rect *dstRect, const FrameBuffer *srcFrameBuffer, int srcX, int srcY, const char *andMask);
  virtual void move(const Rect *dstRect, const int srcX, const int srcY);
  virtual bool cmpFrom(const Rect *dstRect, const FrameBuffer *srcFrameBuffer, const int srcX, const int srcY);
  virtual inline Dimension getDimension() const;
  virtual inline PixelFormat getPixelFormat() const;
  virtual void setProperties(const Dimension* newDim, const PixelFormat* pixelFormat, HWND compatibleWindow);

};