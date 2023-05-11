#pragma once
#include "io_lib/IOException.h"

class EOFException : public IOException
{
public:
  EOFException();
  virtual ~EOFException();
};
