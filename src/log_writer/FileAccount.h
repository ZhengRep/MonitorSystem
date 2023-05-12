#pragma once
#include "utils/CommonHeader.h"
#include "utils/DateTime.h"
#include "thread/LocalMutex.h"
#include "file_lib/WinFile.h"
#include "LogDump.h"

class FileAccount : public LogDump
{
public:
  FileAccount(const TCHAR* logDir, const TCHAR* fileName, unsigned char logLevel, bool logHeadEnabled);
  FileAccount(bool logHeadEnabled);

  virtual ~FileAccount();
  void init(const TCHAR* logDir, const TCHAR* fileName, unsigned char logLevel);
  void changeLogProps(const TCHAR* newLogDir, unsigned char newLevel);
  void getFileName(StringStorage* fileName);
  bool isTheOurFileName(const TCHAR* fileName);
  virtual void print(unsigned int processId, unsigned int threadId,
    const DateTime* dt, int level, const TCHAR* message);

  virtual bool acceptsLevel(int logLevel);

protected:
  virtual void flush(unsigned int processId, unsigned int threadId,
    const DateTime* dt, int level, const TCHAR* message);

private:
  void setNewFile(unsigned char newLevel, const TCHAR* newDir);
  void openFile();
  void closeFile();
  void addUnicodeSignature();
  void createBackup(unsigned int backupLimit);

  void format(unsigned int processId, unsigned int threadId,
    const DateTime* dt, int level, const TCHAR* message);

  void print(int level, const TCHAR* message);

  bool printsLine(int level);

  void updateLogDirPath();

  StringStorage m_logDir;
  StringStorage m_fileName;
  unsigned char m_level;
  bool m_asFirstOpen;
  WinFile* m_file;

  LocalMutex m_logMut;
};

