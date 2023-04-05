#pragma once
#include "InputStream.h"
#include "IOException.h"

class ByteArrayInputStream: public InputStream
{
public:
  ByteArrayInputStream(const char* buffer, size_t bufferSize);
  virtual ~ByteArrayInputStream();
  virtual size_t read(void* buffer, size_t len) throw(IOException);
  virtual size_t available();

protected:
  const char* m_buffer;
  size_t m_bufferSize;
  size_t m_left;
};

