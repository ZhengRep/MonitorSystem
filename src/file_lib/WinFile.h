#pragma once
#include "Utils/CommonHeader.h"

enum DesiredAccess
{
  F_READ,
  F_WRITE,
  F_READ_WRITE
};

enum FileMode
{
  FM_APPEND,
  FM_CREATE,
  FM_CREATE_NEW,
  FM_OPEN,
  FM_OPEN_OR_CREATE,
  FM_TRUNCATE
};

class WinFile
{
public:
  WinFile();
  WinFile(const TCHAR* pathToFile, DesiredAccess dAcc, FileMode fMode, bool shareToRead = false);
  virtual ~WinFile();
  void open(const TCHAR* pathToFile, DesiredAccess dAcc, FileMode fMode, bool shareToRead = false);
  void close();
  virtual size_t read(void* buff, size_t count);
  virtual size_t write(const void* buff, size_t count);
  bool isValid();
  void getPathName(StringStorage* pathName);
  void seek(INT64 n);
  void flush();

  virtual size_t available();

private:
  HANDLE m_hFile;
  StringStorage m_pathToFile;
};

