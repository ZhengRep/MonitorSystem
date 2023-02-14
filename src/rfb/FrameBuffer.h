#pragma once
#include "region/Dimension.h"
#include "rfb/PixelFormat.h"

class FrameBuffer
{
public:
	FrameBuffer(void);
	virtual ~FrameBuffer(void);

	virtual bool assignProperties(const FrameBuffer* srcFrameBuffer);
	virtual bool clone(const FrameBuffer* srcFrameBuffer);
	virtual void setColor(UINT8 red, UINT8 green, UINT8 blue);
	virtual void fillRect(const Rect* dstRect, UINT32 color);

	virtual bool isEqualTo(const FrameBuffer* frameBuffer);

	virtual bool copyFrom(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY);
	virtual bool copyFrom(const FrameBuffer* srcFrameBuffer, int srcX, int srcY);

	virtual bool copyFromRotated90(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY);
	virtual bool copyFromRotated180(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY);
	virtual bool copyFromRotated270(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY);

	virtual bool overlay(const Rect* detRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY, const char* andMask);

	template<class PIXEL_T>
	bool overLayT(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY, const char* andMask);

	virtual void move(const Rect* dstRect, const int srcX, const int srcY);

	virtual bool cmpFrom(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, const int srcX, const int Y);

	virtual bool setDimension(const Dimension* newDim);
	virtual bool setDimension(const Rect* rect)
	{
		Dimension dim(rect);
		return setDimension(&dim);
	}

	virtual void setEmptyDimension(const Rect* dimByRect);
	virtual void setEmptyPixelFmt(const PixelFormat* pf);

	virtual void setPropertiesWithoutResize(const Dimension* newDim, const PixelFormat* pf);

	virtual inline Dimension getDimension() const { return m_dimension; }

	virtual bool setPixelFormat(const PixelFormat* pixelFormat);
	virtual inline PixelFormat getPixelFormat() const { return m_pixelFormat; }
	virtual bool setProperties(const Dimension* newDim, const PixelFormat* pixelFormat);
	virtual bool setProperties(const Rect* dimByRect, const PixelFormat* pixelFormat);
	virtual UINT8 getBitsPerPixel() const;
	virtual UINT8 getBytesPerPixel() const;

	virtual void setBuffer(void* newBuffer) { m_buffer = newBuffer; };

	virtual inline void* getBuffer() const {return m_buffer;}
	virtual void* getBufferPtr(int x, int y) const;
	virtual inline int getBufferSize() const;
	virtual inline int getBytesPerRow() const { return m_dimension.width * m_pixelFormat.bitsPerPixel / 8; }

protected:
	bool resizeBuffer();
	void clipRect(const Rect* dstRect, const FrameBuffer* srcFrameBuffer,const int srcX, const int srcY,Rect* dstClippedRect, Rect* srcClippedRect);
	void clipRect(const Rect* dstRect, const Rect* srcBufferRect,const int srcX, const int srcY,Rect* dstClippedRect, Rect* srcClippedRect);

	template<class PIXEL_T> bool overlayT(const Rect* dstRect, const FrameBuffer* srcFrameBuffer, int srcX, int srcY, const char* andMask);

private:
	Dimension m_dimension;
	PixelFormat m_pixelFormat;
	void* m_buffer;
};
