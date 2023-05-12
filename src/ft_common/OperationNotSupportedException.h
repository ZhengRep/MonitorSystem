#pragma once
#include "utils/Exception.h"

class OperationNotSupportedException: public Exception
{
public:
  OperationNotSupportedException();
  virtual ~OperationNotSupportedException();
};

