#include "ByteArrayInputStream.h"

ByteArrayInputStream::ByteArrayInputStream(const char* buffer, size_t bufferSize)
  : m_buffer(buffer), m_bufferSize(bufferSize), m_left(bufferSize)
{
}

ByteArrayInputStream::~ByteArrayInputStream()
{
}

size_t ByteArrayInputStream::read(void* buffer, size_t len)
{
  if (m_left == 0) {
    throw IOException(_T("End of stream reached"));
  }

  if (len < 0) {
    throw IOException(_T("Negative data length value"));
  }

  char* out = (char*)buffer;
  char* in = (char*)m_buffer;

  size_t bytesToCopy = min(m_left, len);
  size_t outOffset = 0;
  size_t inOffset = m_bufferSize - m_left;

  memcpy(out + outOffset, in + inOffset, bytesToCopy);

  m_left -= bytesToCopy;

  return bytesToCopy;
}

size_t ByteArrayInputStream::available() {
  return m_left;
}
