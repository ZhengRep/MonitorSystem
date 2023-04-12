#pragma once
#include "Utils/CommonHeader.h"

class VncLogFilename
{
public:
  static void queryLogFileDirectory(bool forService, bool shareToAll, StringStorage* logFileDirectory);

private:
  VncLogFilename() {};
};

