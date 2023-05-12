#pragma once
#include "utils/CommonHeader.h"

class ApplicationCrashEvents
{
public:
  virtual void onCrash(const StringStorage* dumpPath) = 0;
};

