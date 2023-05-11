#pragma once
#include "Utils/Exception.h"

class AuthException : public Exception
{
public:
  AuthException(const TCHAR* message) : Exception(message) {}
  virtual ~AuthException() {}
};

