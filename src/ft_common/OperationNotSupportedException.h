#pragma once
#include "Utils/Exception.h"

class OperationNotSupportedException: public Exception
{
public:
  OperationNotSupportedException();
  virtual ~OperationNotSupportedException();
};

