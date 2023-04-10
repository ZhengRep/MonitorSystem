#include "Deflater.h"

Deflater::Deflater()
{
  m_zlibStream.zalloc = Z_NULL;
  m_zlibStream.zfree = Z_NULL;

  deflateInit(&m_zlibStream, Z_DEFAULT_COMPRESSION);

  m_zlibStream.next_in = 0;
  m_zlibStream.avail_in = 0;

  m_zlibStream.next_out = 0;
  m_zlibStream.avail_out = 0;
}

Deflater::~Deflater()
{
  deflateEnd(&m_zlibStream);
}

void Deflater::deflate() throw(ZLibException)
{
  size_t reserve = m_inputSize / 100 + 1024;
  size_t avaliableOutput = m_inputSize + reserve;
  unsigned long prevTotalOut = m_zlibStream.total_out;

  unsigned int constrainedValue = (unsigned int)avaliableOutput;
  _ASSERT(avaliableOutput == constrainedValue);

  m_output.resize(avaliableOutput);

  m_zlibStream.next_in = (Bytef*)m_input;
  m_zlibStream.avail_in = (unsigned int)m_inputSize;

  m_zlibStream.next_out = (Bytef*)&m_output.front();
  m_zlibStream.avail_out = (unsigned int)avaliableOutput;

  if (::deflate(&m_zlibStream, Z_SYNC_FLUSH) != Z_OK) {
    throw ZLibException(_T("Deflate method return error"));
  }

  if (m_zlibStream.avail_in != 0) {
    throw ZLibException(_T("Not enough buffer size for data compression"));
  }

  m_outputSize = m_zlibStream.total_out - prevTotalOut;
}
