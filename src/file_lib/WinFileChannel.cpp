#include "WinFileChannel.h"
#pragma warning(disable: 4996)

WinFileChannel::WinFileChannel(const TCHAR* pathName,
  DesiredAccess dAcc,
  FileMode fMode,
  bool sharedToRead)
{
  m_winFile.open(pathName, dAcc, fMode, sharedToRead);
}

WinFileChannel::~WinFileChannel()
{
  try { close(); } catch (...) {}
}

size_t WinFileChannel::read(void* buffer, size_t len)
{
  return m_winFile.read(buffer, len);
}

size_t WinFileChannel::write(const void* buffer, size_t len)
{
  return m_winFile.write(buffer, len);
}

void WinFileChannel::close()
{
  m_winFile.close();
}

void WinFileChannel::seek(INT64 n)
{
  try {
    m_winFile.seek(n);
  } catch (Exception& e) {
    throw IOException(e.getMessage());
  }
}

size_t WinFileChannel::available() {
  return m_winFile.available();
}

