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

}

void UpdateKeeper::setBorderRect(const Rect* borderRect)
{
}

void UpdateKeeper::setScreenSizeChanged()
{
}

void UpdateKeeper::setCursorPosChanged(const Point* curPos)
{
}

void UpdateKeeper::setCursorPos(const Point* curPos)
{
}

void UpdateKeeper::setCursorShapeChanged()
{
}

void UpdateKeeper::setExcludedRegion(const Region* excludedRegion)
{
}

void UpdateKeeper::addUpdateContainer(const UpdateContainer* updateContainer)
{
}

void UpdateKeeper::getUpdateContainer(UpdateContainer* updCont)
{
}

bool UpdateKeeper::checkForUpdates(const Region* region)
{
    return false;
}

void UpdateKeeper::extract(UpdateContainer* updateContainer)
{
}
