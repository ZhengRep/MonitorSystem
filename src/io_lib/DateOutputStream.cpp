#include "DateOutputStream.h"
#include "Utils/Utf8StringStorage.h"
#include <vector>

#define GETBYTE(x, n) (((x) >> ((n) * 8)) & 0xFF)

DateOutputStream::DateOutputStream(OutputStream* outputStream)
  :m_outputStream(outputStream)
{
}

DateOutputStream::~DateOutputStream()
{
}

size_t DateOutputStream::write(const void* buffer, size_t len) throw(IOException)
{
  return m_outputStream->write(buffer, len);
}

void DateOutputStream::writeFully(const void* buffer, size_t len) throw(IOException)
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

void DateOutputStream::writeUInt8(UINT8 x) throw(IOException)
{
  writeFully((char*)&x, 1);
}

void DateOutputStream::writeUInt16(UINT16 data) throw(IOException)
{
  UINT8 buf[2];

  buf[0] = GETBYTE(data, 1);
  buf[1] = GETBYTE(data, 0);

  writeFully((char*)buf, sizeof(buf));
}

void DateOutputStream::writeUInt32(UINT32 data) throw(IOException)
{
  UINT8 buf[4];

  buf[0] = GETBYTE(data, 3);
  buf[1] = GETBYTE(data, 2);
  buf[2] = GETBYTE(data, 1);
  buf[3] = GETBYTE(data, 0);

  writeFully((char*)buf, sizeof(buf));
}

void DateOutputStream::writeUInt64(UINT64 data) throw(IOException)
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

void DateOutputStream::writeInt8(INT8 data) throw(IOException)
{
  writeUInt8((UINT8)data);
}

void DateOutputStream::writeInt16(INT16 data) throw(IOException)
{
  writeUInt16((UINT16)data);
}

void DateOutputStream::writeInt32(INT32 data) throw(IOException)
{
  writeUInt32((UINT32)data);
}

void DateOutputStream::writeInt64(INT64 data) throw(IOException)
{
  writeUInt64((UINT64)data);
}

void DateOutputStream::writeUTF8(const TCHAR* string) throw(IOException)
{
  Utf8StringStorage utf8String(&StringStorage(string));

  try {
    unsigned int sizeInBytes = (unsigned int)utf8String.getSize();
    _ASSERT(sizeInBytes == utf8String.getSize()); //over uint?
    writeUInt32(sizeInBytes);
    writeFully(utf8String.getString(), sizeInBytes);
  } catch (...) {
    throw;
  }
}

void DateOutputStream::flush() throw(IOException)
{
  m_outputStream->flush();
}
