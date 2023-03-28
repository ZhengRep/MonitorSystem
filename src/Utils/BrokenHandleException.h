#pragma once
#include "Exception.h"

class BrokenHandleException : public Exception
{
public:
  BrokenHandleException(const TCHAR* message) : Exception(message) {};
  ~BrokenHandleException() {};
};