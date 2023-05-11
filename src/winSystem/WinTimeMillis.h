#pragma once
#include "Utils/CommonHeader.h"

class WinTimeMillis
{
public:
  WinTimeMillis(void);
  virtual ~WinTimeMillis(void);

  bool update() {
    return setToCurrentTime();
  }
  int diffFrom(const WinTimeMillis* older) const;
protected:
  bool setToCurrentTime(){
    GetLocalTime(&m_time);
    return true;
  }
  const SYSTEMTIME* getTime() const {
    return &m_time;
  }
  SYSTEMTIME m_time;
};

