#include "PipeServer.h"

PipeServer::PipeServer(const TCHAR* name, unsigned int bufferSize, SecurityAttributes* secAttr, DWORD milliseconds) throw(Exception)
  : m_milliseconds(milliseconds),
    m_secAttr(secAttr),
    m_serverPipe(INVALID_HANDLE_VALUE),
    m_bufferSize(bufferSize)
{
  m_pipeName.format(_T("\\\\.\\pipe\\%s"), name);
  createServerPipe();
}

PipeServer::~PipeServer()
{
  close();
}

NamedPipe* PipeServer::accept() throw(Exception)
{
  if (m_serverPipe == INVALID_HANDLE_VALUE) {
    createServerPipe();
  }

  OVERLAPPED overlapped;
  memset(&overlapped, 0, sizeof(OVERLAPPED));
  overlapped.hEvent = m_winEvent.getHandle();

  if (ConnectNamedPipe(m_serverPipe, &overlapped)) {
    int errCode = GetLastError();
    StringStorage errMess;
    errMess.format(_T("ConnectNamedPipe failed, error code = %d"), errCode);
    throw Exception(errMess.getString());
  } else {
    int errCode = GetLastError();
    switch (errCode) {
    case ERROR_PIPE_CONNECTED:
      break;
    case ERROR_IO_PENDING:
      m_winEvent.waitForEvent(m_milliseconds);
      DWORD cbRet; // Fake
      if (!GetOverlappedResult(m_serverPipe, &overlapped, &cbRet, FALSE)) {
        int errCode = GetLastError();
        StringStorage errMess;
        errMess.format(_T("GetOverlappedResult() failed after the ConnectNamedPipe() call, error code = %d"), errCode);
        throw Exception(errMess.getString());
      }
      break;
    default:
      StringStorage errMess;
      errMess.format(_T("ConnectNamedPipe failed, error code = %d"), errCode);
      throw Exception(errMess.getString());
    }
  }
  // delete is inside ~NamedPipeTransport()
  NamedPipe* result = new NamedPipe(m_serverPipe, m_bufferSize, true);

  m_serverPipe = INVALID_HANDLE_VALUE;

  return result;
}

void PipeServer::close() throw(Exception)
{
  m_winEvent.notify();
}

void PipeServer::waitForConnect(DWORD milliseconds)
{
}

void PipeServer::closeConnection()
{
}

void PipeServer::createServerPipe() throw(Exception)
{
  m_serverPipe = CreateNamedPipe(m_pipeName.getString(),   // pipe name
    PIPE_ACCESS_DUPLEX |      // read/write access
    FILE_FLAG_OVERLAPPED,     // overlapped mode
    PIPE_TYPE_BYTE |          // message type pipe
    PIPE_READMODE_BYTE |      // message-read mode
    PIPE_WAIT,                // blocking mode
    PIPE_UNLIMITED_INSTANCES, // max. instances
    m_bufferSize,             // output buffer size
    m_bufferSize,             // input buffer size
    0,                        // client time-out
    m_secAttr != 0 ?          // security attributes
    m_secAttr->getSecurityAttributes() : 0
  );
  if (m_serverPipe == INVALID_HANDLE_VALUE) {
    StringStorage errMess;
    errMess.format(_T("CreateNamedPipe failed, error code = %d"), GetLastError());
    throw Exception(errMess.getString());
  }
}
