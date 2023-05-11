#include "LogDump.h"

LogEntry::LogEntry(unsigned int processId,
  unsigned int threadId,
  const DateTime* dt,
  int lineLevel,
  const TCHAR* message)
  : m_processId(processId),
  m_threadId(threadId),
  m_dt(*dt),
  m_lineLevel(lineLevel),
  m_message(message)
{
}

LogDump::LogDump(bool logHeadEnabled, bool logDumpEnabled)
  : m_logHeaderStopped(!logHeadEnabled),
  m_logDumpStopped(!logDumpEnabled)
{
  if (logHeadEnabled) {
    m_logHeaderLines.reserve(MAX_LOG_HEADER_SIZE);
  }
  if (logDumpEnabled) {
    m_logDumpLines.reserve(MAX_LOG_DUMP_SIZE);
  }
}

LogDump::~LogDump()
{
}

void LogDump::storeHeader()
{
  m_logHeaderStopped = true;
}

void LogDump::terminateLogDumping()
{
  m_logDumpStopped = true;
  m_logDumpLines.clear();
}

void LogDump::writeLogHeader()
{
  if (m_logHeaderStopped) {
    for (size_t i = 0; i < m_logHeaderLines.size(); i++) {
      flush(m_logHeaderLines[i].m_processId, m_logHeaderLines[i].m_threadId,
        &m_logHeaderLines[i].m_dt, m_logHeaderLines[i].m_lineLevel,
        m_logHeaderLines[i].m_message.getString());
    }
  }
}

void LogDump::writeLogDump()
{
  for (size_t i = 0; i < m_logDumpLines.size(); i++) {
    flush(m_logDumpLines[i].m_processId, m_logDumpLines[i].m_threadId,
      &m_logDumpLines[i].m_dt, m_logDumpLines[i].m_lineLevel,
      m_logDumpLines[i].m_message.getString());
  }
}

void LogDump::updateLogHeaderLines(unsigned int processId,
  unsigned int threadId,
  const DateTime* dt,
  int level,
  const TCHAR* message)
{
  if (logHeadEnabled()) {
    m_logHeaderLines.push_back(LogEntry(processId, threadId, dt, level, message));
  }
}

void LogDump::updateLogDumpLines(unsigned int processId,
  unsigned int threadId,
  const DateTime* dt,
  int level,
  const TCHAR* message)
{
  if (logDumpEnabled()) {
    m_logDumpLines.push_back(LogEntry(processId, threadId, dt, level, message));
  }
}

size_t LogDump::getLogDumpSize()
{
  return m_logDumpLines.size();
}

bool LogDump::logDumpEnabled()
{
  return !m_logDumpStopped && m_logDumpLines.size() < MAX_LOG_DUMP_SIZE;
}

bool LogDump::logHeadEnabled()
{
  return !m_logHeaderStopped && m_logHeaderLines.size() < MAX_LOG_HEADER_SIZE;
}
