#pragma once
#include "UpdateHandler.h"
#include "ScreenDriver.h"
#include "ScreenDriverFactory.h"

class UpdateHandlerImpl : public UpdateHandler, public UpdateListener
{
public:
  UpdateHandlerImpl(UpdateListener* externalUpdateListener, 
    ScreenDriverFactory* scrDriverFactory, LogWriter* log);
  virtual ~UpdateHandlerImpl();

  virtual void extract(UpdateContainer* updateContainer);
  virtual void setFullUpdateRequested(const Region* region);
  bool checkForUpdates(Region* region);

  virtual void setExcludedRegion(const Region* excludedRegion);

private:
  virtual void executeDetectors();
  virtual void terminateDetectors();

  void doUpdate()
  {
    if (m_externalUpdateListener) {
      m_externalUpdateListener->onUpdate();
    }
  }

  virtual void onUpdate();

  void applyNewScreenProperties();

  UpdateKeeper m_updateKeeper;
  ScreenDriver* m_screenDriver;
  UpdateFilter* m_updateFilter;
  UpdateListener* m_externalUpdateListener;
  Rect m_absoluteRect;
  LogWriter* m_log;

  bool m_fullUpdateRequested;
};

