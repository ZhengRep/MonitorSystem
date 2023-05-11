#pragma once
#include "Utils/CommonHeader.h"

class WinServiceEvents
{
public:
  virtual void onSuccServiceStart() = 0;
  virtual void onFailedServiceStart(const StringStorage* reason) = 0;
  virtual void onServiceStop() = 0;
  virtual void enable() = 0;
};

