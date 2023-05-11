#pragma once
#include "RemoteException.h"

class ControlAuthException : public RemoteException
{
public:
  ControlAuthException(const TCHAR* reason, bool silent = false);
  virtual ~ControlAuthException();

  bool isSilent() const;
private:
  bool m_isSilent;
};

