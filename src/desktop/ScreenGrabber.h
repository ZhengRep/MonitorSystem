#pragma once
#include "region/Rect.h"
#include "region/Dimension.h"
#include "region/Point.h"
#include "rfb/PixelFormat.h"
#include "rfb/FrameBuffer.h"

class ScreenGrabber
{
public:
  ScreenGrabber(void);
  virtual ~ScreenGrabber(void);

  virtual bool grab(const Rect* rect = 0) = 0;
  virtual FrameBuffer* getScreenBuffer() { return &m_workFrameBuffer; }
  virtual void setWorkRect(Rect* workRect);
  virtual Rect getWorkRect() const {
    return Rect(m_offsetFrameBuffer.x, m_offsetFrameBuffer.y,
      m_workFrameBuffer.getDimension().width + m_offsetFrameBuffer.x, 
      m_workFrameBuffer.getDimension().height + m_offsetFrameBuffer.y);
  }

  virtual Rect getScreenRect() { return m_fullScreenRect; }

  // Checks screen(desktop) properties on changes
  inline virtual bool getPropertiesChanged() = 0;
  inline virtual bool getPixelFormatChanged() = 0;
  inline virtual bool getScreenSizeChanged() = 0;

  virtual bool applyNewProperties();

protected:
  virtual bool applyNewFullScreenRect() = 0;
  virtual bool applyNewPixelFormat() = 0;

  virtual bool setWorkRectDefault();

  Rect m_fullScreenRect;
  Point m_offsetFrameBuffer;
  FrameBuffer m_workFrameBuffer;
};

