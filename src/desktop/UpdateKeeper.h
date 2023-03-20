#pragma once
#include "thread/LocalMutex.h"
#include "region/Region.h"
#include "UpdateContainer.h"
#include "thread/AutoLock.h"

class UpdateKeeper: public Lockable
{
public:
  UpdateKeeper();
  UpdateKeeper(const Rect* borderRect);
  ~UpdateKeeper();

  virtual void lock() {
    m_updateContainerLocMut.lock();
  }
  virtual void unlock() {
    m_updateContainerLocMut.unlock();
  }

  void addChangedRegion(const Region* changedRegion);
  void addChangedRect(const Rect* changedRect);
  void dazzleChangedReg() {
    AutoLock al(&m_updateContainerLocMut); //why not use LocMut->lock   ¡·¡· because deconstruct
    addChangedRect(&m_borderRect);
  }
  void addCopyRect(const Rect* copyRect, const Point* src);
  void setBorderRect(const Rect* borderRect);

  void setScreenSizeChanged();
  void setCursorPosChanged(const Point* curPos);
  void setCursorPos(const Point* curPos);
  void setCursorShapeChanged();

  void setExcludedRegion(const Region* excludedRegion);

  void addUpdateContainer(const UpdateContainer* updateContainer);
  void getUpdateContainer(UpdateContainer* updCont);
  bool checkForUpdates(const Region* region);

  void extract(UpdateContainer* updateContainer);
private:
  UpdateContainer m_updateContainer;
  LocalMutex m_updateContainerLocMut;

  Rect m_borderRect;

  Region m_excludeRegion;
  LocalMutex m_excludeRegionLocMut;

};

