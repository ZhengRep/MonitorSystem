#include "DibFrameBuffer.h"
#include "winSystem/SystemException.h"

DibFrameBuffer::DibFrameBuffer()
  :m_dibSection(0)
{
}

DibFrameBuffer::~DibFrameBuffer()
{
  releaseDibSection();
}

void DibFrameBuffer::setColor(UINT8 reg, UINT8 green, UINT8 blue)
{
  m_fb.setColor(reg, green, blue);
}

void DibFrameBuffer::fillRect(const Rect* dstRect, UINT32 color)
{
  m_fb.fillRect(dstRect, color);
}

bool DibFrameBuffer::isEqualTo(const FrameBuffer* frameBuffer)
{
  return m_fb.isEqualTo(frameBuffer);
}

bool DibFrameBuffer::copyFrom(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY)
{
  return m_fb.copyFrom(dstRect, srcFrameBuffer, srcX, srcY);
}

bool DibFrameBuffer::copyFrom(const FrameBuffer* srcFrameBuffer, int srcX, int srcY)
{
  return m_fb.copyFrom(srcFrameBuffer, srcX, srcY);
}

bool DibFrameBuffer::overlay(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY, const char* andMask)
{
  return m_fb.overlay(dstRect, srcFrameBuffer, srcX, srcY, andMask);
}

void DibFrameBuffer::move(const Rect* dstRect, const int srcX, const int srcY)
{
  m_fb.move(dstRect, srcX, srcY);
}

bool DibFrameBuffer::cmpFrom(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, const int srcX, const int srcY)
{
  return m_fb.cmpFrom(dstRect, srcFrameBuffer, srcX, srcY);
}

inline Dimension DibFrameBuffer::getDimension() const
{
  throw Exception(_T("Wrong: You shouln't use the DibFrameBuffer::getDimension() function."));
}

inline PixelFormat DibFrameBuffer::getPixelFormat() const
{
  throw Exception(_T("Wrong: You shouln't use the DibFrameBuffer::getPixelFormat() function."));
}

void DibFrameBuffer::setProperties(const Dimension* newDim, const PixelFormat* pixelFormat, HWND compatibleWindow)
{
  throw Exception(_T("Wrong: You shouln't use the DibFrameBuffer::setProperties() function."));
}

void DibFrameBuffer::setTargetDC(HDC targetDC)
{
  checkDibValid();
  m_dibSection->setTargetDC(targetDC);
}

UINT8 DibFrameBuffer::getBitsPerPixel() const
{
  throw Exception(_T("Wrong: You shouln't use the DibFrameBuffer::getBitsPerPixel() function."));
}

bool DibFrameBuffer::clone(const FrameBuffer* srcFrameBuffer)
{
  throw Exception(_T("Wrong: You shouln't use the DibFrameBuffer::clone() function."));
}

bool DibFrameBuffer::setDimension(const Dimension* newDim)
{
  return false;
}

bool DibFrameBuffer::setDimension(const Rect* rect)
{
  return false;
}

void DibFrameBuffer::setEmptyDimension(const Rect* dimByRect)
{
  throw Exception(_T("This function is deprecated"));
}

void DibFrameBuffer::setEmptyPixelFmt(const PixelFormat* pf)
{
  throw Exception(_T("This function is deprecated"));
}

void DibFrameBuffer::setPropertiesWithoutResize(const Dimension* newDim, const PixelFormat* pf)
{
  throw Exception(_T("Wrong: You shouln't use the DibFrameBuffer::setPropertiesWithoutResize() function."));
}

bool DibFrameBuffer::setPixelFormat(const PixelFormat* pixelFormat)
{
  throw Exception(_T("Wrong: You shouln't use the DibFrameBuffer::setPixelFormat() function."));
}

UINT8 DibFrameBuffer::getBytesPerPixel() const
{
  return m_fb.getBytesPerPixel();
}

void DibFrameBuffer::setBuffer(void* newBuffer)
{
  throw Exception(_T("Wrong: You shouln't use the DibFrameBuffer::setBuffer() function."));
}

inline void* DibFrameBuffer::getBuffer() const
{
  return m_fb.getBuffer();
}

void* DibFrameBuffer::getBufferPtr(int x, int y) const
{
  return m_fb.getBufferPtr(x, y);
}

inline int DibFrameBuffer::getBufferSize() const
{
  return m_fb.getBufferSize();
}

inline int DibFrameBuffer::getBytesPerRow() const
{
  return m_fb.getBytesPerRow();
}

void DibFrameBuffer::blitToDibSection(const Rect* rect)
{
  checkDibValid();
  m_dibSection->blitToDibSection(rect);
}

void DibFrameBuffer::blitTransparentToDibSection(const Rect* rect)
{
  checkDibValid();
  m_dibSection->blitTransparentToDibSection(rect);
}

void DibFrameBuffer::blitFromDibSection(const Rect* rect)
{
  checkDibValid();
  m_dibSection->blitFromDibSection(rect);
}

void DibFrameBuffer::stretchFromDibSection(const Rect* srcRect, const Rect* dstRect)
{
  checkDibValid();
  m_dibSection->stretchFromDibSection(srcRect, dstRect);
}

void DibFrameBuffer::setProperties(const Dimension* newDim,   const PixelFormat* pixelFormat,  HWND compatibleWindow)
{
  m_fb.setPropertiesWithoutResize(newDim, pixelFormat);
  void* buffer = updateDibSection(newDim, pixelFormat, compatibleWindow);
  m_fb.setBuffer(buffer);
}

void* DibFrameBuffer::updateDibSection(const Dimension* newDim, const PixelFormat* pixelFormat, HWND compatibleWindow)
{
  releaseDibSection();
  m_dibSection = new DibSection(pixelFormat, newDim, compatibleWindow);
  return m_dibSection->getBuffer();
}

void DibFrameBuffer::releaseDibSection()
{
  if (m_dibSection) {
    delete m_dibSection;
    m_dibSection = 0;
    m_fb.setBuffer(0);
  }
}

void DibFrameBuffer::checkDibValid()
{
  if (m_dibSection == 0) {
    throw Exception(_T("Can't set target DC because it is not initialized a DIB section yet"));
  }
}

