#include "ByteArrayOutputStream.h"
#include <string>

ByteArrayOutputStream::ByteArrayOutputStream(size_t max)
  :m_size(0), m_max(max), m_ownMemory(true)
{
  m_buffer = new char[m_max];
}

ByteArrayOutputStream::ByteArrayOutputStream()
  : m_size(0), m_buffer(0), m_max(DEFAULT_INNER_BUFFER_CAPACITY), m_ownMemory(true)
{
  m_buffer = new char[m_max];
}

ByteArrayOutputStream::ByteArrayOutputStream(void* alienMemory)
  : m_size(0), m_buffer((char*)alienMemory), m_max(0xFFFFFF), m_ownMemory(false)
{
}

ByteArrayOutputStream::~ByteArrayOutputStream()
{
  if ((m_buffer != 0) && (m_ownMemory)) {
    delete[] m_buffer;
  }
}

size_t ByteArrayOutputStream::write(const void* buffer, size_t len)
{
  bool allocateNewBuffer = (m_size + len) > m_max;

  if (allocateNewBuffer && m_ownMemory) {
    size_t reserve = DEFAULT_INNER_BUFFER_CAPACITY;
    char* newBuffer = new char[m_size + len + reserve];
    memcpy(newBuffer, m_buffer, m_size);
    delete[] m_buffer;
    m_buffer = newBuffer;
    m_max = m_size + len + reserve;
  }

  memcpy(&m_buffer[m_size], (const char*)buffer, len);
  m_size += len;

  return len;
}

size_t ByteArrayOutputStream::size() const
{
  return m_size;
}

const char* ByteArrayOutputStream::toByteArray() const
{
  return m_buffer;
}
