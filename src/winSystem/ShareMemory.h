#pragma once
#include "Utils/CommonHeader.h"

class SharedMemory
{
public:
  SharedMemory(const TCHAR* name, size_t size);
  virtual ~SharedMemory();
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

