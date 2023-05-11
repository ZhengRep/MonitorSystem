#pragma once
#include "FileChannel.h"
#include "WinFile.h"

class WinFileChannel: public FileChannel
{
public:
  WinFileChannel(const TCHAR* pathName, DesiredAccess dAcc, FileMode fMode, bool sharedToRead = true);
  ~WinFileChannel();
  virtual void seek(INT64 n);
  virtual size_t read(void* buffer, size_t len);
  virtual size_t write(const void* buffer, size_t len);
  virtual void close() throw(Exception);
  size_t available();

private:
  WinFile m_winFile;
};

