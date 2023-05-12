#include "FileLogger.h"

FileLogger::FileLogger(const TCHAR* logDir, const TCHAR* fileName,
  unsigned char logLevel, bool logHeadEnabled)
  : m_fileAccount(logDir, fileName, logLevel, logHeadEnabled)
{
}

FileLogger::FileLogger(bool logHeadEnabled)
  : m_fileAccount(logHeadEnabled)
{
}

FileLogger::~FileLogger()
{
}

void FileLogger::init(const TCHAR* logDir, const TCHAR* fileName, unsigned char logLevel)
{
  m_fileAccount.init(logDir, fileName, logLevel);
}

void FileLogger::storeHeader()
{
  m_fileAccount.storeHeader();
}

void FileLogger::print(int logLevel, const TCHAR* line)
{
  try {
    UINT32 processId = GetCurrentProcessId();
    UINT32 threadId = GetCurrentThreadId();
    DateTime currTime = DateTime::now();

    m_fileAccount.print(processId, threadId, &currTime, logLevel, line);
  } catch (...) {
  }
}

bool FileLogger::acceptsLevel(int logLevel)
{
  return m_fileAccount.acceptsLevel(logLevel);
}

void FileLogger::changeLogProps(const TCHAR* newLogDir, unsigned char newLevel)
{
  m_fileAccount.changeLogProps(newLogDir, newLevel);
}
