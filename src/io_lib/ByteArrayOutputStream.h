#pragma once
#include "OutputStream.h"

class ByteArrayOutputStream: public OutputStream
{
public:
  static const size_t DEFAULT_INNER_BUFFER_CAPACITY = 1024;
public:
  ByteArrayOutputStream(size_t max);
  ByteArrayOutputStream();
  ByteArrayOutputStream(void* alienMemory);
  virtual ~ByteArrayOutputStream();
  virtual size_t write(const void* buffer, size_t len);
  size_t size() const;
  const char* toByteArray() const;

protected:
  bool m_ownMemory;
  char* m_buffer;
  size_t m_size;
  size_t m_max;
};

