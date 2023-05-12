#pragma once

#include "rfb/FrameBuffer.h"
#include "win_system/DibSection.h"

class DibFrameBuffer : public FrameBuffer
{
 public:
  DibFrameBuffer();
  virtual ~DibFrameBuffer();
  virtual void setColor(UINT8 reg, UINT8 green, UINT8 blue);
  virtual void fillRect(const Rect* dstRect, UINT32 color);
  virtual bool isEqualTo(const FrameBuffer* frameBuffer);
  virtual bool copyFrom(const Rect *dstRect, const FrameBuffer *srcFrameBuffer, int srcX, int srcY);
  virtual bool copyFrom(const FrameBuffer *srcFrameBuffer, int srcX, int srcY);
  virtual bool overlay(const Rect *dstRect, const FrameBuffer *srcFrameBuffer, int srcX, int srcY, const char *andMask);
  virtual void move(const Rect *dstRect, const int srcX, const int srcY);
  virtual bool cmpFrom(const Rect *dstRect, const FrameBuffer *srcFrameBuffer, const int srcX, const int srcY);
  virtual inline Dimension getDimension() const;
  virtual inline PixelFormat getPixelFormat() const;
  virtual void setProperties(const Dimension* newDim, const PixelFormat* pixelFormat, HWND compatibleWindow);
  void setTargetDC(HDC targetDC);
  virtual UINT8 getBitsPerPixel() const;
  virtual UINT8 getBytesPerPixel() const;
  virtual inline void* getBuffer() const;
  virtual inline void* getBufferPtr(int x, int y) const;
  virtual inline int getBufferSize() const;
  virtual inline int getBytesPerRow() const;

  void blitToDibSection(const Rect* rect);
  void blitTransparentToDibSection(const Rect* rect);
  void blitFromDibSection(const Rect* rect);
  void stretchFromDibSection(const Rect* srcRect, const Rect* dstRect);
  
private:
  virtual bool assignProperties(const FrameBuffer* srcFrameBuffer);
  virtual bool clone(const FrameBuffer* srcFrameBuffer);
  virtual bool setDimension(const Dimension* newDim);
  virtual bool setDimension(const Rect* rect);
  virtual void setEmptyDimension(const Rect* dimByRect);
  virtual bool setPixelFormat(const PixelFormat* pixelFormat);
  virtual void setEmptyPixelFmt(const PixelFormat* pf);
  virtual bool setProperties(const Dimension* newDim, const PixelFormat* pixelFormat);
  virtual bool setProperties(const Rect* dimByRect, const PixelFormat* pixelFormat);
  virtual void setPropertiesWithoutResize(const Dimension* newDim, const PixelFormat* pf);
  virtual void setBuffer(void* newBuffer);

  void* updateDibSection(const Dimension* newDim, const PixelFormat* pixelFormat, HWND compatibleWindow);
  void releaseDibSection();
  void checkDibValid();

  FrameBuffer m_fb;
  DibSection* m_dibSection;
};