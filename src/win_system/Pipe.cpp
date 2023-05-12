#include "Pipe.h"
#include "thread/AutoLock.h"
#include "win_system/Environment.h"

unsigned int Pipe::getMaxPortionSize()
{
  return m_maxPortionSize;
}

Pipe::Pipe(unsigned int maxPortionSize)
  :m_totalRead(0), m_totalWrote(0), m_maxPortionSize(maxPortionSize)
{
}

Pipe::~Pipe()
{
}

size_t Pipe::readByHandle(void* buffer, size_t len, HANDLE pipeHandle)
{
  DWORD result = 0;
  OVERLAPPED overlapped;
  memset(&overlapped, 0, sizeof(OVERLAPPED));
  overlapped.hEvent = m_readEvent.getHandle();

  bool success;
  DWORD length = (DWORD)len;
  _ASSERT(length == len);
  if (m_maxPortionSize > 0 && length > m_maxPortionSize) {
    length = m_maxPortionSize;
  }

  {
    AutoLock l(&m_hPipeMutex);
    checkPipeHandle(pipeHandle);
    success = ReadFile(pipeHandle, buffer, length, &result, &overlapped) != 0;
  }

  if (!success) {
    DWORD errCode = GetLastError();

    if (errCode == ERROR_IO_PENDING) {
      m_readEvent.waitForEvent();
      DWORD cbRet = 0;
      AutoLock al(&m_hPipeMutex);
      checkPipeHandle(pipeHandle);
      if (GetOverlappedResult(pipeHandle, &overlapped, &cbRet, FALSE) &&
        cbRet != 0) {
        result = cbRet;
      } else {
        StringStorage errText;
        errText.format(_T("The Pipe's read function failed after GetOverlappedResult calling (pipe handle is %p)"), pipeHandle);
        StringStorage errMess;
        Environment::getErrStr(errText.getString(), &errMess);
        throw IOException(errMess.getString());
      }
    } else {
      StringStorage errText;
      errText.format(_T("The Pipe's write function failed after ReadFile calling pipe handle is %p, total read %llu, try to read %u)"),
        pipeHandle, m_totalRead, length);
      StringStorage errMess;
      Environment::getErrStr(errText.getString(), &errMess);
      throw IOException(errMess.getString());
    }
  } // else operation already successful has been completed

  if (result == 0) {
    throw IOException(_T("Unknown pipe error"));
  }
  m_totalRead += result;
  return result;
}

size_t Pipe::writeByHandle(void* buffer, size_t len, HANDLE pipeHandle)
{
  DWORD result;
  OVERLAPPED overlapped;
  memset(&overlapped, 0, sizeof(OVERLAPPED));
  overlapped.hEvent = m_writeEvent.getHandle();

  bool success;
  DWORD length = (DWORD)len;
  _ASSERT(length == len);
  if (m_maxPortionSize != 0 && length > m_maxPortionSize) {
    length = m_maxPortionSize;
  }
  {
    AutoLock al(&m_hPipeMutex);
    checkPipeHandle(pipeHandle);
    success = WriteFile(pipeHandle, buffer, length, &result, &overlapped) != 0;
  }

  if (!success) {
    int errCode = GetLastError();

    if (errCode == ERROR_IO_PENDING) {
      m_writeEvent.waitForEvent();
      DWORD cbRet;
      AutoLock al(&m_hPipeMutex);
      checkPipeHandle(pipeHandle);
      if (GetOverlappedResult(pipeHandle, &overlapped, &cbRet, FALSE) || cbRet == 0) {
        result = cbRet;
      } else {
        StringStorage errMess;
        Environment::getErrStr(_T("The Pipe's write function failed after GetOverlappedResult calling"), &errMess);
        throw IOException(errMess.getString());
      }
    } else {
      StringStorage errText;
      errText.format(_T("The Pipe's write function failed after WriteFile calling pipe handle is %p, total write %llu, try to write %u)"),
        pipeHandle, m_totalWrote, length);
      StringStorage errMess;
      Environment::getErrStr(errText.getString(), &errMess);
      throw IOException(errMess.getString());
    }
  } // else operation already successful has been completed

  if (result == 0) {
    throw IOException(_T("Unknown pipe error"));
  }

  m_totalWrote += result;
  return result;
}

void Pipe::checkPipeHandle(HANDLE pipeHandle)
{
  if (pipeHandle == INVALID_HANDLE_VALUE) {
    throw IOException(_T("Invalid pipe handle"));
  }
}
