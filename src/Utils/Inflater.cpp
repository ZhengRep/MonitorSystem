#include "Inflater.h"

Inflater::Inflater()
  :m_unpackedSize(0)
{
  m_zlibStream.zalloc = Z_NULL;
  m_zlibStream.zfree = Z_NULL;

  inflateInit(&m_zlibStream);

  m_zlibStream.next_in = 0;
  m_zlibStream.avail_in = 0;

  m_zlibStream.next_out = 0;
  m_zlibStream.avail_out = 0;
}

Inflater::~Inflater()
{
  inflateEnd(&m_zlibStream);
}

void Inflater::setUnpackedSize(size_t size)
{
  m_unpackedSize = size;
}

void Inflater::inflate() throw(ZLibException)
{
  size_t avaliableOutput = m_unpackedSize + m_unpackedSize / 100 + 1024;
  unsigned long prevTotalOut = m_zlibStream.total_out;

  // Check to overflow.
  unsigned int constrainedValue = (unsigned int)avaliableOutput;
  _ASSERT(avaliableOutput == constrainedValue);
  constrainedValue = (unsigned int)m_inputSize;
  _ASSERT(m_inputSize == constrainedValue);

  m_output.resize(avaliableOutput);

  m_zlibStream.next_in = (Bytef*)m_input;
  m_zlibStream.avail_in = (unsigned int)m_inputSize;

  m_zlibStream.next_out = (Bytef*)&m_output.front();
  m_zlibStream.avail_out = (unsigned int)avaliableOutput;

  int r = ::inflate(&m_zlibStream, Z_SYNC_FLUSH);

  if (r == Z_STREAM_END) {
    throw ZLibException(_T("ZLib stream end"));
  }
  if (r == Z_NEED_DICT) {
    throw ZLibException(_T("ZLib need dictionary"));
  }
  if (r == Z_STREAM_ERROR) {
    throw ZLibException(_T("ZLib stream error"));
  }
  if (r == Z_MEM_ERROR) {
    throw ZLibException(_T("ZLib memory error"));
  }
  if (r == Z_DATA_ERROR) {
    throw ZLibException(_T("Zlib data error"));
  }
  if (m_zlibStream.avail_in != 0) {
    throw ZLibException(_T("Not enough buffer size for data decompression"));
  }

  m_outputSize = m_zlibStream.total_out - prevTotalOut;
}
