#include "CursorShape.h"

CursorShape::~CursorShape()
{
}

bool CursorShape::clone(const CursorShape* srcCursorShape)
{
  m_hotSpot = srcCursorShape->getHotSpot();
  if (m_pixels.clone(srcCursorShape->getPixels())) {
    m_mask = srcCursorShape->m_mask;
    return true;
  } else {
    return false;
  }
}

bool CursorShape::setDimension(const Dimension* newDim)
{
  bool result = m_pixels.setDimension(newDim);
  return result && resizeBuffer();
}

bool CursorShape::setPixelFormat(const PixelFormat* pixelFormat)
{
  bool result = m_pixels.setPixelFormat(pixelFormat);
  return result && resizeBuffer();
}

bool CursorShape::setProperties(const Dimension* newDim, const PixelFormat* pixelFormat)
{
  bool result = m_pixels.setDimension(newDim) && m_pixels.setPixelFormat(pixelFormat);
  return result && resizeBuffer();
}

void CursorShape::assignMaskFromRfb(const char* srcMask)
{
  if (!m_mask.empty()) {
    int height = m_pixels.getDimension().height;
    size_t rfbWidthInBytes = getMaskWidthInBytes();
    size_t maskLen = height * rfbWidthInBytes;
    memcpy(&m_mask.front(), srcMask, maskLen);
  }
}

void CursorShape::assignMaskFromWindows(const char* srcMask)
{
  if (!m_mask.empty()) {
    int height = m_pixels.getDimension().height;
    int winWidthInBytes = ((m_pixels.getDimension().width + 15) / 16) * 2;
    int rfbWidthInBytes = getMaskWidthInBytes();
    for (int i = 0; i < height; i++) {
      memcpy(&m_mask[i * rfbWidthInBytes],
        &srcMask[i * winWidthInBytes],
        rfbWidthInBytes);
    }
  }
}

int CursorShape::getMaskSize() const
{
  return getMaskWidthInBytes() * m_pixels.getDimension().height;
}

int CursorShape::getMaskWidthInBytes() const
{
  return (m_pixels.getDimension().width + 7) / 8;
}

void CursorShape::resetToEmpty()
{
  setDimension(&(Dimension(0, 0)));
  setHotSpot(0, 0);
}

bool CursorShape::resizeBuffer()
{
  m_mask.resize(getMaskSize());
  return true;
}
