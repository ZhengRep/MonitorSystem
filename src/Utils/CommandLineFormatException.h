#pragma once
#include "Exception.h"

class CommandLineFormatException : public Exception
{
public:
  CommandLineFormatException(const TCHAR* message = _T("Invalid command line format."));
  virtual ~CommandLineFormatException();
};

