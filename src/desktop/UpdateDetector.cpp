#include "UpdateDetector.h"

UpdateDetector::UpdateDetector(UpdateKeeper* updateKeeper, UpdateListener* updateListener)
  :m_updateKeeper(updateKeeper), m_updateListener(updateListener)
{
}

UpdateDetector::~UpdateDetector()
{
}
