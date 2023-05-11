#pragma once
#include "Utils/CommonHeader.h"

class ApplicationCrashEvents
{
public:
  virtual void onCrash(const StringStorage* dumpPath) = 0;
};

