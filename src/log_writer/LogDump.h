#pragma once
#include "Utils/DateTime.h"

struct LogEntry
{
  LogEntry(unsigned int processId, unsigned int threadId, const DateTime* dt,
    int lineLevel, const TCHAR* message);

  unsigned int m_processId;
  unsigned int m_threadId;
  DateTime m_dt;
  int m_lineLevel;
  StringStorage m_message;
};

class LogDump
{
public:
  LogDump(bool logHeadEnabled, bool logDumpEnabled);
  virtual ~LogDump();

  virtual void flush(unsigned int processId, unsigned int threadId,
    const DateTime* dt, int level, const TCHAR* message) = 0;

  void storeHeader();
  size_t getLogDumpSize();

protected:
  void updateLogHeaderLines(unsigned int processId, unsigned int threadId,
    const DateTime* dt, int level, const TCHAR* message);

  void updateLogDumpLines(unsigned int processId, unsigned int threadId,
    const DateTime* dt, int level, const TCHAR* message);

  void terminateLogDumping();
  void writeLogHeader();

  void writeLogDump();
  bool logDumpEnabled();
  bool logHeadEnabled();

private:
  static const size_t MAX_LOG_HEADER_SIZE = 16;
  bool m_logHeaderStopped;
  std::vector<LogEntry> m_logHeaderLines;

  static const size_t MAX_LOG_DUMP_SIZE = 1024;
  bool m_logDumpStopped;
  std::vector<LogEntry> m_logDumpLines;
};

