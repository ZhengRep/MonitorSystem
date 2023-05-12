#include "AnonymousPipe.h"
#include "win_system/Environment.h"
#include "thread/AutoLock.h"

AnonymousPipe::AnonymousPipe(HANDLE hWrite, HANDLE hRead, unsigned int maxPortionSize, LogWriter* log)
  : Pipe(maxPortionSize),
  m_hWrite(hWrite),
  m_hRead(hRead),
  m_neededToClose(true),
  m_log(log)
{
}

AnonymousPipe::~AnonymousPipe()
{
  try{
    close();
  } catch (Exception& e) {
    m_log->error(_T("The close() function failed at AnonymousPipe destructor: %s"), e.getMessage());
  }
}

void AnonymousPipe::close()
{
  AutoLock al(&m_hPipeMutex);

  bool wrSuc = true;
  bool rdSuc = true;
  StringStorage wrErrText, rdErrText;
  if (m_hWrite != INVALID_HANDLE_VALUE && m_neededToClose) {
    if (CloseHandle(m_hWrite) == 0) {
      Environment::getErrStr(_T("Cannot close anonymous pipe write handle."), &wrErrText);
      wrSuc = false;
    }
    m_log->debug(_T("Closed m_hWrite(%p) AnonymousPipe handle"), m_hWrite);
  }
  m_hWrite = INVALID_HANDLE_VALUE;
  if (m_hRead != INVALID_HANDLE_VALUE && m_neededToClose) {
    if (CloseHandle(m_hRead) == 0) {
      Environment::getErrStr(_T("Cannot close anonymous pipe read handle."), &wrErrText);
      rdSuc = false;
    }
    m_log->debug(_T("Closed m_hRead(%p) AnonymousPipe handle"), m_hRead);
  }
  m_hRead = INVALID_HANDLE_VALUE;
  if (!wrSuc || !rdSuc) {
    StringStorage errMess;
    errMess.format(_T("AnonymousPipe::close() funciton has failed (%s %s)"), wrErrText.getString(), rdErrText.getString());
    throw Exception(errMess.getString());
  }
}

size_t AnonymousPipe::read(void* buffer, size_t len) throw(IOException)
{
  try {
    return readByHandle(buffer, len, m_hRead);
  } catch (...) {
    m_log->error(_T("AnonymousPipe::read() failed (m_hRead = %p)"), m_hRead);
    throw;
  }
}

size_t AnonymousPipe::write(const void* buffer, size_t len) throw(IOException)
{
  try {
    return writeByHandle((void*)buffer, len, m_hWrite);
  } catch (...) {
    m_log->error(_T("AnonymousPipe::write() failed (m_hWrite = %p)"), m_hWrite);
    throw;
  }
}

HANDLE AnonymousPipe::getWriteHandle() const
{
  return m_hWrite;
}

HANDLE AnonymousPipe::getReadHandle() const
{
  return m_hRead;
}

void AnonymousPipe::assignHandlesFor(HANDLE hTargetProc, bool neededToClose, bool keepCloseRight)
{
  HANDLE hSrcProc = GetCurrentProcess();
  HANDLE hWrite = 0, hRead = 0;
  if (DuplicateHandle(hSrcProc, m_hWrite, hTargetProc, &hWrite, 0, FALSE, DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS) == 0) {
    StringStorage errText;
    Environment::getErrStr(_T("Cannot dupplicate write handle for the anonymous pipe"), &errText);
    throw Exception(errText.getString());
  }
  m_hWrite = hWrite;
  if (DuplicateHandle(hSrcProc, m_hRead, hTargetProc, &hRead, 0, FALSE, DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS) == 0) {
    StringStorage errText;
    Environment::getErrStr(_T("Cannot dupplicate read handle for the anonymous pipe"), &errText);
    throw Exception(errText.getString());
  }
  m_hRead = hRead;
  // Try keep of the close rights.
  if (keepCloseRight) {
    if (DuplicateHandle(hTargetProc, m_hWrite, 0, 0, 0, FALSE, DUPLICATE_CLOSE_SOURCE) == 0) {
      StringStorage errText;
      Environment::getErrStr(_T("Cannot keep the right to close of the write handle of the anonymous pipe"), &errText);
      throw Exception(errText.getString());
    }
    if (DuplicateHandle(hTargetProc, m_hRead, 0, 0, 0, FALSE, DUPLICATE_CLOSE_SOURCE) == 0) {
      StringStorage errText;
      Environment::getErrStr(_T("Cannot keep the right to close of the read handle of the anonymous pipe"),&errText);
      throw Exception(errText.getString());
    }
    // Now the current process can close the handles.
  }
  m_neededToClose = neededToClose;
}

void AnonymousPipe::setTimeOut(unsigned int timeOut)
{
  m_timeOut = timeOut;
}

void AnonymousPipe::checkPipeHandle(HANDLE handle)
{
  if (handle == INVALID_HANDLE_VALUE) {
    throw IOException(_T("Invalid pipe handle"));
  }
}
