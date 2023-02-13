#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(void)
{
}

FrameBuffer::~FrameBuffer(void)
{
}

bool FrameBuffer::assignProperties(const FrameBuffer* srcFrameBuffer)
{
    return false;
}

bool FrameBuffer::clone(const FrameBuffer* srcFrameBuffer)
{
    return false;
}

void FrameBuffer::setColor(UINT8 red, UINT8 green, UINT8 blue)
{
}

void FrameBuffer::fillRect(const Rect* dstRect, UINT32 color)
{
}

bool FrameBuffer::isEqualTo(const FrameBuffer* frameBuffer)
{
    return false;
}

bool FrameBuffer::copyFrom(const Rect* dstRect, const FrameBuffer* srcFrameBuffer)
{
    return false;
}

bool FrameBuffer::copyFrom(const FrameBuffer* srcFrameBuffer, int srcX, int srcY)
{
    return false;
}

bool FrameBuffer::copyFromRotated90(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY)
{
    return false;
}

bool FrameBuffer::copyFromRotated180(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY)
{
    return false;
}

bool FrameBuffer::copyFromRotated270(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY)
{
    return false;
}

bool FrameBuffer::overlay(const Rect* detRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY, const char* andMask)
{
    return false;
}

void FrameBuffer::move(const Rect* dstRect, const int srcX, const int srcY)
{
}

bool FrameBuffer::cmpFrom(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, const int srcX, const int Y)
{
    return false;
}

bool FrameBuffer::setDimension(const Dimension* newDim)
{
    return false;
}

void FrameBuffer::setEmptyDimension(const Rect* dimByRect)
{
}

void FrameBuffer::setEmptyPixelFmt(const PixelFormat* pf)
{
}

void FrameBuffer::setPropertiesWithoutResize(const Dimension* newDim, const PixelFormat* pf)
{
}

bool FrameBuffer::setPixelFormat(const PixelFormat* pixelFormat)
{
    return false;
}

bool FrameBuffer::setProperties(const Dimension* newDim, const PixelFormat* pixelFormat)
{
    return false;
}

bool FrameBuffer::setProperties(const Rect* dimByRect, const PixelFormat* pixelFormat)
{
    return false;
}

UINT8 FrameBuffer::getBitsPerPixel() const
{
    return UINT8();
}

UINT8 FrameBuffer::getBytesPerPixel() const
{
    return UINT8();
}

void* FrameBuffer::getBufferPtr(int x, int y) const
{
    return nullptr;
}

inline int FrameBuffer::getBufferSize() const
{
    return 0;
}

bool FrameBuffer::resizeBuffer()
{
    return false;
}

void FrameBuffer::clipRect(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, const int srcX, const int srcY, Rect* dstClippedRect, Rect* srcClippedRect)
{
}

void FrameBuffer::clipRect(const Rect* dstRect, const Rect* srcBufferRect, const int srcX, const int srcY, Rect* dstClippedRect, Rect* srcClippedRect)
{
}
