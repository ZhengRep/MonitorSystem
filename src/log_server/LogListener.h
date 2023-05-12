#pragma once
#include "FileLogDefs.h"
#include "utils/DateTime.h"

class LogListener
{
public:
  virtual void onLog(FileAccountHandle handle, unsigned int processId,
    unsigned int threadId, const DateTime* dt, int level, const TCHAR* message) = 0;
  virtual void onAnErrorFromLogConn(const TCHAR* message) = 0;
};

