#pragma once
#include "UpdateContainer.h"
#include "UpdateKeeper.h"
#include "UpdateFilter.h"
#include "ScreenGrabber.h"
#include "WindowsCursorShapeGrabber.h"
#include "rfb/FrameBuffer.h"
#include "thread/AutoLock.h"
#include "UpdateListener.h"
#include "UpdateDetector.h"
#include "CopyRectDetector.h"
#include "desktop_ipc/BlockingGate.h"

class UpdateHandler
{
public:
  UpdateHandler();
  virtual ~UpdateHandler(void);
  virtual void extract(UpdateContainer* updateContainer) = 0;
  virtual void setFullUpdateRequested(const Region* region) = 0;
  virtual bool checkForUpdates(Region* region) = 0;
  virtual void setExcludedRegion(const Region* excludedRegion) = 0;
  const FrameBuffer* getFrameBuffer() const { return &m_backupFrameBuffer; }
  const CursorShape* getCursorShape() const { return &m_cursorShape; }

  void getFrameBufferProp(Dimension* dim, PixelFormat* pf)
  {
    AutoLock al(&m_fbLocMut);
    *dim = m_backupFrameBuffer.getDimension();
    *pf = m_backupFrameBuffer.getPixelFormat();
  }

  Dimension getFrameBufferDimension()
  {
    AutoLock al(&m_fbLocMut);
    return m_backupFrameBuffer.getDimension();
  }

  PixelFormat getFrameBufferPixelFormat(Dimension* dim, PixelFormat* pf)
  {
    AutoLock al(&m_fbLocMut);
    return m_backupFrameBuffer.getPixelFormat();
  }

  void initFrameBuffer(const FrameBuffer* newFb);

  virtual bool updateExternalFrameBuffer(FrameBuffer* fb, const Region* region, const Rect* viewPort);
  virtual void sendInit(BlockingGate* gate) {}

protected:
  virtual bool updateExternalFrameBuffer(FrameBuffer* dstFb, FrameBuffer* srcFb,
    const Region* region, const Rect* viewPort);

  FrameBuffer m_backupFrameBuffer;
  LocalMutex m_fbLocMut;
  CursorShape m_cursorShape;
};

