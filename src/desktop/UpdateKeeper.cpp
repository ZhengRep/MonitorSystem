#include "UpdateKeeper.h"

UpdateKeeper::UpdateKeeper()
{
}

UpdateKeeper::UpdateKeeper(const Rect* borderRect)
{
  m_borderRect.setRect(borderRect);
}

UpdateKeeper::~UpdateKeeper()
{
}

void UpdateKeeper::addChangedRegion(const Region* changedRegion)
{
  AutoLock al(&m_updateContainerLocMut);
  m_updateContainer.changedRegion.add(changedRegion);
  m_updateContainer.changedRegion.crop(&m_borderRect);
}

void UpdateKeeper::addChangedRect(const Rect* changedRect)
{
  Region region(changedRect);
  addChangedRegion(&region);
}

void UpdateKeeper::addCopyRect(const Rect* copyRect, const Point* src)
{
  AutoLock al(&m_updateContainerLocMut);
  if (copyRect->isEmpty()) {
    return;
  }

  Region* changedRegion = &m_updateContainer.changedRegion;
  Region* copiedRegion = &m_updateContainer.copiedRegion;
  Point* copySrc = &m_updateContainer.copySrc;
  Rect dstCopyRect(copyRect);

  Rect srcCopyRect(copyRect);
  srcCopyRect.setLocation(src->x, src->y);

  //clip dstCopyRect
  dstCopyRect = dstCopyRect.intersection(&m_borderRect);
  //correct source coordinates
  srcCopyRect.left += dstCopyRect.left - copyRect->left;
  srcCopyRect.top += dstCopyRect.top - copyRect->top;
  srcCopyRect.right += dstCopyRect.right - copyRect->right;
  srcCopyRect.bottom += dstCopyRect.bottom - copyRect->bottom;
  // Clipping srcCopyRect
  Rect dummySrcCopyRect(&srcCopyRect);
  srcCopyRect = srcCopyRect.intersection(&m_borderRect);
  // Correcting destination coordinates
  dstCopyRect.left += srcCopyRect.left - dummySrcCopyRect.left;
  dstCopyRect.top += srcCopyRect.top - dummySrcCopyRect.top;
  dstCopyRect.right += srcCopyRect.right - dummySrcCopyRect.right;
  dstCopyRect.bottom += srcCopyRect.bottom - dummySrcCopyRect.bottom;

  if(dstCopyRect.isEmpty()) {
    return;
  }

  copySrc->x = srcCopyRect.left;
  copySrc->y = srcCopyRect.top;

  Region diff(copyRect);
  Region dstCopyRegion(&dstCopyRect);
  diff.subtract(&dstCopyRegion);
  addChangedRegion(&diff);

  if (!copiedRegion->isEmpty()) {
    changedRegion->add(copiedRegion);
    copiedRegion->clear();
    addChangedRect(copyRect);
    return;
  }

  copiedRegion->clear();
  copiedRegion->addRect(&dstCopyRect);

  changedRegion->subtract(copiedRegion);

  Region addonChangedRegion(&srcCopyRect);
  addonChangedRegion.intersect(changedRegion);

  addonChangedRegion.translate(dstCopyRect.left - copySrc->x, dstCopyRect.top - copySrc->y);
  changedRegion->add(&addonChangedRegion);

  // Clipping regions
  m_updateContainer.changedRegion.crop(&m_borderRect);
  m_updateContainer.copiedRegion.crop(&m_borderRect);
}

void UpdateKeeper::setBorderRect(const Rect* borderRect)
{
  AutoLock al(&m_updateContainerLocMut);
  m_borderRect = *borderRect;
}

void UpdateKeeper::setScreenSizeChanged()
{
  AutoLock al(&m_updateContainerLocMut);
  m_updateContainer.screenSizeChanged = true;
}

void UpdateKeeper::setCursorPosChanged(const Point* curPos)
{
  AutoLock al(&m_updateContainerLocMut);
  m_updateContainer.cursorPosChanged = true;
  m_updateContainer.cursorPos = *curPos;
}

void UpdateKeeper::setCursorPos(const Point* curPos)
{
  AutoLock al(&m_updateContainerLocMut);
  m_updateContainer.cursorPos = *curPos;
}

void UpdateKeeper::setCursorShapeChanged()
{
  AutoLock al(&m_updateContainerLocMut);
  m_updateContainer.cursorShapeChanged = true;
}

void UpdateKeeper::setExcludedRegion(const Region* excludedRegion)
{
  AutoLock al(&m_excludeRegionLocMut);

  if (excludedRegion == 0) {
    m_excludeRegion.clear();
  } else {
    m_excludeRegion = *excludedRegion;
  }
}

void UpdateKeeper::addUpdateContainer(const UpdateContainer* updateContainer)
{
  AutoLock al(&m_updateContainerLocMut);

  std::vector<Rect> rects;
  std::vector<Rect>::iterator iRect;
  updateContainer->copiedRegion.getRectVector(&rects);
  size_t numRects = rects.size();
  if (numRects > 0) {
    iRect = rects.begin();
    addCopyRect(&(*iRect), &updateContainer->copySrc);
  }

  // Add changed region
  addChangedRegion(&updateContainer->changedRegion);

  // Add video region
  m_updateContainer.videoRegion.add(&updateContainer->videoRegion);

  // Set other properties
  if (updateContainer->screenSizeChanged) {
    setScreenSizeChanged();
  }
  setCursorPos(&updateContainer->cursorPos);
  if (updateContainer->cursorPosChanged) {
    setCursorPosChanged(&updateContainer->cursorPos);
  }
  if (updateContainer->cursorShapeChanged) {
    setCursorShapeChanged();
  }
}

void UpdateKeeper::getUpdateContainer(UpdateContainer* updateContainer)
{
  AutoLock al(&m_updateContainerLocMut);
  *updateContainer = m_updateContainer;
}

bool UpdateKeeper::checkForUpdates(const Region* region)
{
  UpdateContainer updateContainer;
  getUpdateContainer(&updateContainer);

  Region resultRegion = updateContainer.changedRegion;
  resultRegion.add(&updateContainer.copiedRegion);
  resultRegion.intersect(region);

  bool result = updateContainer.cursorPosChanged || updateContainer.cursorShapeChanged || updateContainer.screenSizeChanged || !resultRegion.isEmpty();
  return result;
}

void UpdateKeeper::extract(UpdateContainer* updateContainer)
{
  {
    AutoLock al(&m_updateContainerLocMut);

    // Clipping regions
    m_updateContainer.changedRegion.crop(&m_borderRect);
    m_updateContainer.copiedRegion.crop(&m_borderRect);

    *updateContainer = m_updateContainer;
    m_updateContainer.clear();
  }
  {
    AutoLock al(&m_updateContainerLocMut);
    updateContainer->changedRegion.subtract(&m_excludeRegion);
    updateContainer->copiedRegion.subtract(&m_excludeRegion);
  }
}
