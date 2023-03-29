#include "BufferedOutputStream.h"

BufferedOutputStream::BufferedOutputStream(OutputStream* output)
  :m_dataLength(0)
{
  m_output = new DataOutputStream(output);
}

BufferedOutputStream::~BufferedOutputStream()
{
  try {
    flush();
  } catch (...) {

  }

  delete m_output;
}

size_t BufferedOutputStream::write(const void* buffer, size_t len) throw(IOException)
{
  if (m_dataLength + len >= sizeof(m_buffer)) {
    flush();
    m_output->writeFully(buffer, len);
  } else {
    memcpy(&m_buffer[m_dataLength], buffer, len);

    m_dataLength += len;
  }

  return len;
}

void BufferedOutputStream::flush() throw(IOException)
{
  m_output->writeFully(&m_buffer[0], m_dataLength);
  m_dataLength = 0;
}
