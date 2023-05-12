#pragma once
#include "FileAccount.h"
#include "Logger.h"

class FileLogger : public Logger
{
public:
  FileLogger(const TCHAR* logDir, const TCHAR* fileName, unsigned char logLevel, bool logHeadEnabled);
  FileLogger(bool logHeadEnabled);
  virtual ~FileLogger();
  void init(const TCHAR* logDir, const TCHAR* fileName, unsigned char logLevel);
  void changeLogProps(const TCHAR* newLogDir, unsigned char newLevel);
  void storeHeader();
  virtual void print(int logLevel, const TCHAR* line);

  virtual bool acceptsLevel(int logLevel);

private:
  FileAccount m_fileAccount;
};

