#include "ControlAuthException.h"

ControlAuthException::ControlAuthException(const TCHAR* reason, bool isSilent)
  : RemoteException(reason), m_isSilent(isSilent)
{
}

ControlAuthException::~ControlAuthException()
{
}

bool ControlAuthException::isSilent() const
{
  return m_isSilent;
}
