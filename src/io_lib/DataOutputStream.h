#pragma once
#include "Utils/inttypes.h"
#include "OutputStream.h"
#include "IOException.h"

class DataOutputStream: public OutputStream
{
public:
  DataOutputStream(OutputStream* outputStream);
  virtual ~DataOutputStream();
  //Inherited from superclass, just delegate call to real output stream
  virtual size_t write(const void* buffer, size_t len) throw(IOException);
  void writeFully(const void* buffer, size_t len) throw(IOException);

  void writeUInt8(UINT8 data) throw(IOException);
  void writeUInt16(UINT16 data) throw(IOException);
  void writeUInt32(UINT32 data) throw(IOException);
  void writeUInt64(UINT64 data) throw(IOException);

  void writeInt8(INT8 data) throw(IOException);
  void writeInt16(INT16 data) throw(IOException);
  void writeInt32(INT32 data) throw(IOException);
  void writeInt64(INT64 data) throw(IOException);

  void writeUTF8(const TCHAR* string) throw(IOException);

  virtual void flush() throw(IOException);
protected:
  OutputStream* m_outputStream;
};

