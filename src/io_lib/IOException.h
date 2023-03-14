#pragma once

#include "Utils/Exception.h"

class IOException: public Exception
{
public:
  IOException();
  IOException(const TCHAR* message);
  virtual ~IOException();
  
};

