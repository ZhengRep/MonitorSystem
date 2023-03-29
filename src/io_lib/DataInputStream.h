#pragma once
#include "Utils/inttypes.h"
#include "InputStream.h"
#include "IOException.h"

class DataInputStream: public InputStream
{
public:
  DataInputStream(InputStream* inputStream);
  virtual ~DataInputStream();
  virtual size_t read(void* buffer, size_t len) throw(IOException);
  virtual size_t available();

  void readFully(void* buffer, size_t len) throw(IOException);

  UINT8 readUInt8() throw(IOException);
  UINT16 readUInt16() throw(IOException);
  UINT32 readUInt32() throw(IOException);
  UINT64 readUInt64() throw(IOException);

  INT8 readInt8() throw(IOException);
  INT16 readInt16() throw(IOException);
  INT32 readInt32() throw(IOException);
  INT64 readInt64() throw(IOException);

  void readUTF8(StringStorage* storage) throw(IOException);

protected:
  InputStream* m_inputStream;
};

