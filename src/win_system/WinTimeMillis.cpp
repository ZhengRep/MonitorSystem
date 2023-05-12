#include "WinTimeMillis.h"

WinTimeMillis::WinTimeMillis(void)
{
  memset(&m_time, 0, sizeof(m_time));
}

WinTimeMillis::~WinTimeMillis(void)
{
}

int WinTimeMillis::diffFrom(const WinTimeMillis* older) const
{
  int olderTime, newTime;
  const SYSTEMTIME* oldTime;
  
  oldTime = older->getTime();

  olderTime = oldTime->wHour * 360000 + oldTime->wMinute * 60000 
    + oldTime->wSecond * 1000 + oldTime->wMilliseconds;

  newTime = m_time.wHour * 3600000 + m_time.wMinute * 60000
    + m_time.wSecond * 1000 + m_time.wMilliseconds;

  return newTime - olderTime;
}
