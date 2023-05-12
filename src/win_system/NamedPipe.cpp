#include "NamedPipe.h"
#include "thread/AutoLock.h"

NamedPipe::NamedPipe(HANDLE hPipe, unsigned int maxPortionSize, bool asServer)
  :Pipe(maxPortionSize), 
  m_hPipe(hPipe),
  m_asServer(asServer)
{
}

NamedPipe::~NamedPipe()
{
  try {
    close();
  } catch (...) {
  }
}

void NamedPipe::close()
{
  AutoLock al(&m_hPipeMutex);
  if (m_asServer) {
    if (DisconnectNamedPipe(m_hPipe) == 0) {
      DWORD errCode = GetLastError();
      StringStorage errMess;
      errMess.format(_T("DisconnectNamedPipe failed, error code = %u"), errCode);
      throw Exception(errMess.getString());
    }
  }

  if (m_hPipe != INVALID_HANDLE_VALUE) {
    CloseHandle(m_hPipe);
    m_hPipe = INVALID_HANDLE_VALUE;
  }
  // Unblock a blocked operation
  m_readEvent.notify();
  m_writeEvent.notify();
}

size_t NamedPipe::read(void* buffer, size_t len) throw(IOException)
{
  return readByHandle(buffer, len, m_hPipe);
}

size_t NamedPipe::write(const void* buffer, size_t len) throw(IOException)
{
  return writeByHandle((void*)buffer, len, m_hPipe);
}

HANDLE NamedPipe::getHandle() const
{
  return m_hPipe;
}

void NamedPipe::checkPipeHandle()
{
  if (m_hPipe == INVALID_HANDLE_VALUE) {
    throw IOException(_T("Invalid pipe handle"));
  }
}
