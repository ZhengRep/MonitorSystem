#pragma once
#include "OutputStream.h"
#include "DataOutputStream.h"

class BufferedOutputStream: public OutputStream
{
public:
  BufferedOutputStream(OutputStream* output);
  virtual ~BufferedOutputStream();

  virtual size_t write(const void* buffer, size_t len) throw(IOException);
  void flush() throw(IOException);

protected:
  DataOutputStream* m_output;
  char m_buffer[100000];
  size_t m_dataLength;
};

