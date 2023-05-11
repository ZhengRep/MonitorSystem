#pragma once
#include "UpdateKeeper.h"
#include "thread/GuiThread.h"
#include "UpdateListener.h"

class UpdateDetector: public GuiThread
{
public:
  UpdateDetector(UpdateKeeper* updateKeeper, UpdateListener* updateListener);
  virtual ~UpdateDetector();

  void setUpdateKeeper(UpdateKeeper* updateKeeper) { m_updateKeeper = updateKeeper; }
  UpdateKeeper* getUpdateKeeper() const { return m_updateKeeper; }

protected:
  void doUpdate() {
    if (m_updateListener) {
      m_updateListener->onUpdate();
    }
  }

  UpdateKeeper* m_updateKeeper;
  UpdateListener* m_updateListener;
};

