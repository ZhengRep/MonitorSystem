#pragma once
#include "thread/LocalMutex.h"
#include "log_writer/LogWriter.h"
#include "utils/CommonHeader.h"
#include <vector>

typedef std::vector<TCHAR*> StringContainer;

class WinEventLog
{
public:
  WinEventLog(LogWriter* log);
  virtual ~WinEventLog();

  void enable();

  void reportInfo(unsigned int messageId, const TCHAR* fmt, ...);
  void reportWarning(unsigned int messageId, const TCHAR* fmt, ...);
  void reportError(unsigned int messageId, const TCHAR* fmt, ...);

private:
  void reportEvent(unsigned int messageId, WORD eventType, 
    const TCHAR* fmt,va_list argList);
  void registerEventSource();
  void deRegisterEventSource();
  void updateEventSourcesSubkey();

  HANDLE getLogHandle();

  HANDLE m_hEventLog;
  LocalMutex m_hEventLogMutex;
  LogWriter* m_log;
};

