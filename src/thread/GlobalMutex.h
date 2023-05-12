#pragma once
#include "utils/CommonHeader.h"
#include "utils/Exception.h"
#include "Lockable.h"

class GlobalMutex : public Lockable
{
public:
  GlobalMutex(const TCHAR* name = 0, bool interSession = false, bool throwIfExist = false) throw(Exception);

  virtual ~GlobalMutex();
  virtual void lock();
  virtual void unlock();

private:
  void setAccessToAll(HANDLE objHandle);

  HANDLE m_mutex;
};

