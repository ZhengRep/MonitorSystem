#pragma once
#include "DateTime.h"

class DemandTimer
{
public:
  DemandTimer(UINT64 intervalMillis);
  ~DemandTimer();

  void reset();
  bool isElapsed();

private:
  UINT64 m_intervalMillis;
  DateTime m_startTime;
};

