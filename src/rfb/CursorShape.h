#pragma once
#include "FrameBuffer.h"
#include "region/Point.h"

class CursorShape
{
public:
  CursorShape():m_mask(0){}
  ~CursorShape();
  bool clone(const CursorShape* srcCursorShape);
  bool setDimension(const Dimension* newDim);
  Dimension getDimension() const { return m_pixels.getDimension(); }

  bool setPixelFormat(const PixelFormat* pixelFormat);
  PixelFormat getPixelFormat() const { return m_pixels.getPixelFormat(); }

  bool setProperties(const Dimension* newDim, const PixelFormat* pixelFormat);
  const FrameBuffer* getPixels() const { return &m_pixels; }
  int getPixelsSize() const { return m_pixels.getBufferSize(); }
  const char* getMask() const { return m_mask.empty() ? 0 : &m_mask.front(); }
  void assignMaskFromRfb(const char* srcMask);
  void assignMaskFromWindows(const char* srcMask);
  int getMaskSize() const;
  int getMaskWidthInBytes() const;

  void setHotSpot(int x, int y) { m_hotSpot.x = x; m_hotSpot.y = y; }
  Point getHotSpot() const { return m_hotSpot; }

  void resetToEmpty();
private:
  bool resizeBuffer();
  FrameBuffer m_pixels;
  std::vector<char> m_mask;
  Point m_hotSpot;
};

