#include "DemandTimer.h"

DemandTimer::DemandTimer(UINT64 intervalMillis)
  : m_intervalMillis(intervalMillis)
{
  reset();
}

DemandTimer::~DemandTimer()
{
}

void DemandTimer::reset()
{
  m_startTime = DateTime::now();
}

bool DemandTimer::isElapsed()
{
  return (DateTime::now() - m_startTime).getTime() >= m_intervalMillis;
}
