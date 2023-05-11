#pragma once
#include "Utils/DateTime.h"
#include "thread/LocalMutex.h"

class AuthTracker
{
public:
  AuthTracker(UINT64 failureTimeInterval, unsigned int failureMaxCount);
  virtual ~AuthTracker();

protected:
  UINT64 checkBan();
  void notifyAbAuthFailed();

private:
  void refresh();

  UINT64 m_failureTimeInterval;

  DateTime m_firstFailureTime;
  unsigned int m_failureCount;
  unsigned int m_failureMaxCount;
  LocalMutex m_countMutex;
};

