#pragma once
#include "utils/DateTime.h"

class InputBlocker
{
public:
  InputBlocker();
  virtual ~InputBlocker();

  // This functions set/unset blocks on a local keyboard and mouse.
  virtual void setKeyboardBlocking(bool block) = 0;
  virtual void setMouseBlocking(bool block) = 0;

  // This functions set/unset blocks on a local keyboard and mouse on the
  // timeInterval interval from a last software event generation.
  virtual void setSoftKeyboardBlocking(bool block, unsigned int timeInterval) = 0;
  virtual void setSoftMouseBlocking(bool block, unsigned int timeInterval) = 0;

  virtual DateTime getLastInputTime() const = 0;
  virtual void correctLastTime(DateTime newTime) = 0;

  virtual bool isRemoteInputAllowed() = 0;
};

