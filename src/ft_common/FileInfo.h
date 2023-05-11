#pragma once
#include "Utils/StringStorage.h"
#include "Utils/inttypes.h"
#include "file_lib/File.h"

class FileInfo
{
public:
  const static int DIRECTORY = 0x1;
  const static int EXECUTABLE = 0x2;

  FileInfo();
  FileInfo(UINT64 size, UINT64 modTime, UINT16 flags, const TCHAR* fileName);
  FileInfo(const File* file);
  bool isDirectory() const;
  bool isExecutable() const;
  void setLastModified(UINT64 time);
  void setSize(UINT64 size);
  void setFlags(UINT16 flags);
  void setFileName(const TCHAR* fileName);
  UINT64 lastModified() const;
  UINT64 getSize() const;
  UINT16 getFlags() const;
  const TCHAR* getFileName() const;

protected:
  UINT64 m_sizeInBytes;
  UINT64 m_lastModified;
  UINT64 m_flags;
  StringStorage m_fileName;
};

