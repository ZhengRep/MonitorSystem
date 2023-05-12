#pragma once
#include "utils/Exception.h"

class RemoteException : public Exception
{
public:
  RemoteException(const TCHAR* reason);
  virtual ~RemoteException();
};

