#pragma once
#include "Exception.h"

class ZLibException: public Exception
{
public:
  ZLibException(const TCHAR* message);
  ZLibException();
  virtual ~ZLibException();
};

