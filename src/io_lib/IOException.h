#pragma once

#include "utils/Exception.h"

class IOException: public Exception
{
public:
  IOException();
  IOException(const TCHAR* message);
  virtual ~IOException();
  
};

