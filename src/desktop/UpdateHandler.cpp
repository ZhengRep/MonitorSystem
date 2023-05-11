#include "UpdateHandler.h"
#include "Utils/Macros.h"

UpdateHandler::UpdateHandler()
{
}

UpdateHandler::~UpdateHandler(void)
{
}

void UpdateHandler::initFrameBuffer(const FrameBuffer* newFb)
{
  AutoLock al(&m_fbLocMut);
  m_backupFrameBuffer.clone(newFb);
}

bool UpdateHandler::updateExternalFrameBuffer(FrameBuffer* fb, const Region* region, const Rect* viewPort)
{
  AutoLock al(&m_fbLocMut);
  return updateExternalFrameBuffer(fb, &m_backupFrameBuffer, region, viewPort);
}

bool UpdateHandler::updateExternalFrameBuffer(FrameBuffer* dstFb, FrameBuffer* srcFb, const Region* region, const Rect* viewPort)
{
  PixelFormat dstPf = dstFb->getPixelFormat();
  PixelFormat srcPf = srcFb->getPixelFormat();
  Dimension dstFbDim = dstFb->getDimension();
  Rect srcFbRect = srcFb->getDimension().getRect();
  Rect resultViewPort = srcFbRect.intersection(viewPort);

  if (!dstPf.isEqualTo(&srcPf) || !dstFbDim.isEqualTo(&unmove(Dimension(&resultViewPort))) ||
    !resultViewPort.isEqualTo(viewPort)) {
    dstFb->setProperties(&resultViewPort, &srcPf);
    return false;
  }

  std::vector<Rect> rects;
  std::vector<Rect>::iterator iRect;
  region->getRectVector(&rects);

  for (iRect = rects.begin(); iRect < rects.end(); iRect++) {
    Rect* rect = &(*iRect);
    dstFb->copyFrom(rect, srcFb,
      rect->left + viewPort->left,
      rect->top + viewPort->top);
  }
  return true;
}
