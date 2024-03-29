#include "DataInputStream.h"
#include "utils/Utf8StringStorage.h"

#define SETBYTE(y, n) (((y) & 0xFF) << ((n) * 8))

DataInputStream::DataInputStream(InputStream* inputStream)
  : m_inputStream(inputStream)
{
}

DataInputStream::~DataInputStream()
{
}

size_t DataInputStream::available()
{
  return m_inputStream->available();
}

size_t DataInputStream::read(void* buffer, size_t len)
{
  return m_inputStream->read(buffer, len);
}

void DataInputStream::readFully(void* buffer, size_t len) throw(IOException)
{
  char* typedBuffer = (char*)buffer;
  size_t totalRead = 0;
  size_t left = len;
  while (totalRead < len) {
    size_t read = m_inputStream->read(typedBuffer + totalRead, left);
    left -= read;
    totalRead += read;
  }
}

UINT8 DataInputStream::readUInt8()
{
  UINT8 x;
  readFully(&x, 1);
  return x;
}

UINT16 DataInputStream::readUInt16()
{
  UINT16 x = 0;
  UINT8 buf[2];
  readFully(&buf[0], 2);
  x += SETBYTE(buf[0], 1);
  x += SETBYTE(buf[1], 0);
  return x;
}

UINT32 DataInputStream::readUInt32()
{
  UINT32 x = 0;
  UINT8 buf[4];
  readFully(&buf[0], 4);
  x += SETBYTE(buf[0], 3);
  x += SETBYTE(buf[1], 2);
  x += SETBYTE(buf[2], 1);
  x += SETBYTE(buf[3], 0);
  return x;
}

UINT64 DataInputStream::readUInt64()
{
  UINT64 x = 0;
  UINT8 buf[8];
  readFully(&buf[0], 8);
  x += (UINT64)buf[0] << (7 * 8);
  x += (UINT64)buf[1] << (6 * 8);
  x += (UINT64)buf[2] << (5 * 8);
  x += (UINT64)buf[3] << (4 * 8);
  x += (UINT64)buf[4] << (3 * 8);
  x += (UINT64)buf[5] << (2 * 8);
  x += (UINT64)buf[6] << (1 * 8);
  x += (UINT64)buf[7] << (0 * 8);
  return x;
}

INT8 DataInputStream::readInt8()
{
  INT8 x;
  readFully(&x, 1);
  return x;
}

INT16 DataInputStream::readInt16()
{
  return (INT16)readUInt16();
}

INT32 DataInputStream::readInt32()
{
  return (INT32)readUInt32();
}

INT64 DataInputStream::readInt64()
{
  return (INT64)readUInt64();
}

void DataInputStream::readUTF8(StringStorage* storage)
{
  UINT32 sizeInBytes = readUInt32();
  if (sizeInBytes > 0) {
    std::vector<char> buffer(sizeInBytes);
    readFully(&buffer.front(), sizeInBytes);
    Utf8StringStorage utf8String(&buffer);
    utf8String.toStringStorage(storage);
  } else {
    storage->setString(_T(""));
  }
}

