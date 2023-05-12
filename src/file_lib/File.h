#pragma once
#include "utils/CommonHeader.h"
#include "utils/inttypes.h"

class File
{
public:
  const static int MODE_READ = 1;
  const static int MODE_WRITE = 2;

  File(const TCHAR* pathname);
  File(const TCHAR* parent, const TCHAR* child);
  ~File();

  bool canRead() const;
  bool canWrite() const;

  bool createNewFile() const;
  bool remove() const;
  bool exists() const;
  void getName(StringStorage* name) const;
  void getFileExtension(StringStorage* ext) const;
  bool isFile() const;
  bool isDirectory() const;
  void getPath(StringStorage* pathname) const;

  /**
   * Returns the last modification time of file denoted by this pathname.
   * @return count of milliseconds since unix epoch or 0 on fail.
   */
  UINT64 lastModified() const;
  UINT64 length() const;
  bool list(StringStorage* fileList, UINT32* filesCount) const;
  static bool listRoots(StringStorage* rootList, UINT32* rootsCount);
  bool mkdir() const;
  bool renameTo(const TCHAR* destPathName);
  bool renameTo(File* dest);
  static bool renameTo(const TCHAR* dest, const TCHAR* source);
  bool setLastModified(INT64 time);
  bool truncate();

private:
  bool getFileInfo(WIN32_FIND_DATA* fileInfo) const;
  bool tryCreateFile(DWORD desiredAccess, DWORD creationDisposition) const;

public:
  static TCHAR s_separatorChar;

protected:
  StringStorage m_pathName;
};

