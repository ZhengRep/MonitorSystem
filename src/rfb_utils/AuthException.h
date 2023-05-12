#pragma once
#include "utils/Exception.h"

class AuthException : public Exception
{
public:
  AuthException(const TCHAR* message) : Exception(message) {}
  virtual ~AuthException() {}
};

