#pragma once
#include "Utils/Exception.h"

class RemoteException : public Exception
{
public:
  RemoteException(const TCHAR* reason);
  virtual ~RemoteException();
};

