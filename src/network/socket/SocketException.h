#pragma once
#include "utils/Exception.h"

class SocketException: public Exception
{
public:
  SocketException();
  SocketException(int error);
  SocketException(const TCHAR* message);
  virtual ~SocketException();
  int errno();

protected:
  void setErrno(int error);
  int m_errno;
};

