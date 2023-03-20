#include "UpdateContainer.h"

UpdateContainer::UpdateContainer()
{
  clear();
}

UpdateContainer::~UpdateContainer()
{
}

UpdateContainer& UpdateContainer::operator=(const UpdateContainer& src)
{
    // TODO: insert return statement here
  copiedRegion = src.copiedRegion;
  changedRegion = src.changedRegion;
  videoRegion = src.videoRegion;
  screenSizeChanged = src.screenSizeChanged;
  cursorPosChanged = src.cursorPosChanged;
  cursorShapeChanged = src.cursorShapeChanged;
  copySrc = src.copySrc;
  cursorPos = src.cursorPos;

  return *this;
}

void UpdateContainer::clear()
{
  copiedRegion.clear();
  changedRegion.clear();
  videoRegion.clear();
  screenSizeChanged = false;
  cursorPosChanged = false;
  cursorShapeChanged = false;
  copySrc.clear();
  cursorPos.clear();
}

bool UpdateContainer::isEmpty() const
{
  return copiedRegion.isEmpty() &&
    changedRegion.isEmpty() &&
    videoRegion.isEmpty() &&
    !screenSizeChanged &&
    !cursorPosChanged &&
    !cursorShapeChanged;
}
