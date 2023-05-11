#include "AuthTracker.h"
#include "thread/AutoLock.h"

AuthTracker::AuthTracker(UINT64 failureTimeInterval, unsigned int failureMaxCount)
  : m_failureCount(0),
  m_failureTimeInterval(failureTimeInterval),
  m_failureMaxCount(failureMaxCount)
{
}

AuthTracker::~AuthTracker()
{
}

UINT64 AuthTracker::checkBan()
{
  refresh();

  UINT64 banTime = 0;
  {
    AutoLock al(&m_countMutex);
    if (m_failureCount >= m_failureMaxCount) {
      banTime = max(0, m_failureTimeInterval - (DateTime::now() - m_firstFailureTime).getTime());
    }
  }
  return banTime;
}

void AuthTracker::notifyAbAuthFailed()
{
  AutoLock al(&m_countMutex);
  if (m_failureCount == 0) {
    m_firstFailureTime = DateTime::now();
  }
  m_failureCount++;
}

void AuthTracker::refresh()
{
  AutoLock al(&m_countMutex);
  if ((DateTime::now() - m_firstFailureTime).getTime() >= m_failureTimeInterval) {
    m_failureCount = 0;
  }
}

