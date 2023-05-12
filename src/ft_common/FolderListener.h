#pragma once
#include "utils/inttypes.h"
#include "FileInfo.h"

class FolderListener
{
public:
  FolderListener(const TCHAR* folderPath);
  ~FolderListener();

  const FileInfo* getFilesInfo() const;
  UINT32 getFilesCount() const;
  bool list();

protected:
  StringStorage m_folderPath;
  FileInfo* m_filesInfo;
  UINT32 m_filesCount;
};

