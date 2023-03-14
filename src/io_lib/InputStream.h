#pragma once
#include "IOException.h"

class InputStream
{
public:
  virtual ~InputStream();

  virtual size_t read(void* buffer, size_t len) = 0;
  virtual size_t available() = 0;
};

