#pragma once
#include "utils/CommonHeader.h"

class MntLogFilename
{
public:
  static void queryLogFileDirectory(bool forService, bool shareToAll, StringStorage* logFileDirectory);

private:
  MntLogFilename() {};
};

