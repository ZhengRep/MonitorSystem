#include "PipeClient.h"

NamedPipe* PipeClient::connect(const TCHAR* name, unsigned int maxPortionSize) throw(Exception)
{
  StringStorage pipeName;
  pipeName.format(_T("\\\\.\\pipe\\%s"), name);

  HANDLE hPipe;
  hPipe = CreateFile(pipeName.getString(),  // pipe name
    GENERIC_READ |         // read and write access
    GENERIC_WRITE,
    0,                     // no sharing
    NULL,                  // default security attributes
    OPEN_EXISTING,         // opens existing pipe
    FILE_FLAG_OVERLAPPED,  // asynchronous mode
    NULL);                 // no template file

  if (hPipe == INVALID_HANDLE_VALUE) {
    int errCode = GetLastError();
    StringStorage errMess;
    errMess.format(_T("Connect to pipe server failed, error code = %d"), errCode);
    throw Exception(errMess.getString());
  }

  DWORD dwMode = PIPE_READMODE_BYTE;
  if (!SetNamedPipeHandleState(hPipe,   // pipe handle
    &dwMode,   // new pipe mode
    NULL,      // don't set maximum bytes
    NULL)      // don't set maximum time
    ) {
    int errCode = GetLastError();
    StringStorage errMess;
    errMess.format(_T("SetNamedPipeHandleState failed, error code = %d"), errCode);
    throw Exception(errMess.getString());
  }

  return new NamedPipe(hPipe, maxPortionSize, false);
}
