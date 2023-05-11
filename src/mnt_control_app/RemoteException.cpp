#include "RemoteException.h"

RemoteException::RemoteException(const TCHAR* reason)
  :Exception(reason)
{
}

RemoteException::~RemoteException()
{
}
