#include "DataOutputStream.h"
#include "utils/Utf8StringStorage.h"
#include <vector>

#define GETBYTE(x, n) (((x) >> ((n) * 8)) & 0xFF)

DataOutputStream::DataOutputStream(OutputStream* outputStream)
  :m_outputStream(outputStream)
{
}

DataOutputStream::~DataOutputStream()
{
}

size_t DataOutputStream::write(const void* buffer, size_t len) throw(IOException)
{
  return m_outputStream->write(buffer, len);
}

void DataOutputStream::writeFully(const void* buffer, size_t len) throw(IOException)
{
  char* typedBuffer = (char*)buffer;
  size_t totalWritten = 0;
  size_t left = len;
  while (totalWritten < len) {
    size_t written = m_outputStream->write(typedBuffer + totalWritten, left);
    left -= written;
    totalWritten += written;
  }
}

void DataOutputStream::writeUInt8(UINT8 x) throw(IOException)
{
  writeFully((char*)&x, 1);
}

void DataOutputStream::writeUInt16(UINT16 data) throw(IOException)
{
  UINT8 buf[2];

  buf[0] = GETBYTE(data, 1);
  buf[1] = GETBYTE(data, 0);

  writeFully((char*)buf, sizeof(buf));
}

void DataOutputStream::writeUInt32(UINT32 data) throw(IOException)
{
  UINT8 buf[4];

  buf[0] = GETBYTE(data, 3);
  buf[1] = GETBYTE(data, 2);
  buf[2] = GETBYTE(data, 1);
  buf[3] = GETBYTE(data, 0);

  writeFully((char*)buf, sizeof(buf));
}

void DataOutputStream::writeUInt64(UINT64 data) throw(IOException)
{
  UINT8 buf[8];

  buf[0] = GETBYTE(data, 7);
  buf[1] = GETBYTE(data, 6);
  buf[2] = GETBYTE(data, 5);
  buf[3] = GETBYTE(data, 4);
  buf[4] = GETBYTE(data, 3);
  buf[5] = GETBYTE(data, 2);
  buf[6] = GETBYTE(data, 1);
  buf[7] = GETBYTE(data, 0);

  writeFully((char*)buf, sizeof(buf));
}

void DataOutputStream::writeInt8(INT8 data) throw(IOException)
{
  writeUInt8((UINT8)data);
}

void DataOutputStream::writeInt16(INT16 data) throw(IOException)
{
  writeUInt16((UINT16)data);
}

void DataOutputStream::writeInt32(INT32 data) throw(IOException)
{
  writeUInt32((UINT32)data);
}

void DataOutputStream::writeInt64(INT64 data) throw(IOException)
{
  writeUInt64((UINT64)data);
}

void DataOutputStream::writeUTF8(const TCHAR* string) throw(IOException)
{
  StringStorage str = StringStorage(string);
  Utf8StringStorage utf8String(&str);

  try {
    unsigned int sizeInBytes = (unsigned int)utf8String.getSize();
    _ASSERT(sizeInBytes == utf8String.getSize()); //over uint?
    writeUInt32(sizeInBytes);
    writeFully(utf8String.getString(), sizeInBytes);
  } catch (...) {
    throw;
  }
}

void DataOutputStream::flush() throw(IOException)
{
  m_outputStream->flush();
}
