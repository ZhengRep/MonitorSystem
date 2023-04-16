#pragma once
#include "io_lib/IOException.h"

class ReconnectException: public IOException
{
public:
  ReconnectException(const TCHAR* message): IOException(message) {};
  virtual ~ReconnectException(){};
};

