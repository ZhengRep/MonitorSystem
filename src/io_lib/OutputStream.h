#pragma once
#include "IOException.h"

class OutputStream
{
public:
  virtual ~OutputStream();
  virtual size_t write(const void* buffer, size_t len) = 0;
  virtual void flush();
};

