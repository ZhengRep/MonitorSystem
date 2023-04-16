#pragma once
#include "Utils/CommonHeader.h"

class ShareMemory
{
public:
  ShareMemory(const TCHAR* name, size_t size);
  virtual ~ShareMemory();
  void* getMemPointer() { return m_memory; }

protected:
  bool createFile(const TCHAR* name, size_t size);
  void mapViewOfFile();
  void setAllAccess(HANDLE objHandle);
  void freeRes();

private:
  void* m_memory;
  HANDLE m_hToMap;
};

