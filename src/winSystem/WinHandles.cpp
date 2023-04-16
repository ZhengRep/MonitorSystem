#include "WinHandles.h"
#include "Environment.h"
#include "Utils/Exception.h"

HANDLE WinHandles::assignHandleFor(HANDLE hSource, HANDLE hTargetProc, bool neededToCloseSource, bool keepCloseRight)
{
  HANDLE hSrcProc = GetCurrentProcess();
  HANDLE hDest = 0;
  DWORD options = DUPLICATE_SAME_ACCESS;
  if (neededToCloseSource) {
    options |= DUPLICATE_CLOSE_SOURCE;
  }
  if (DuplicateHandle(hSrcProc, hSource, hTargetProc, &hDest, 0, FALSE, options) == 0) {
    StringStorage errText;
    Environment::getErrStr(&errText);
    throw Exception(errText.getString());
  }
  // Try keep of the close rights.
  if (keepCloseRight) {
    if (DuplicateHandle(hTargetProc, hDest, 0, 0, 0, FALSE, DUPLICATE_CLOSE_SOURCE) == 0) {
      StringStorage errText;
      Environment::getErrStr(&errText);
      throw Exception(errText.getString());
    }
  }
  return hDest;
}

HANDLE WinHandles::assignHandleFor(HANDLE hSource, unsigned int procId, bool neededToCloseSource, bool keepCloseRight)
{
  HANDLE processHandle = OpenProcess(PROCESS_DUP_HANDLE, FALSE, procId);
  if (processHandle == 0) {
    throw Exception(_T("Couldn't open process to assign a handle"));
  }
  HANDLE dstHandle;
  try {
    dstHandle = assignHandleFor(hSource, processHandle, neededToCloseSource, keepCloseRight);
    CloseHandle(processHandle);
    return dstHandle;
  } catch (...) {
    CloseHandle(processHandle);
    throw;
  }
}
