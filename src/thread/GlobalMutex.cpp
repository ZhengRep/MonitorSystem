#include "GlobalMutex.h"
#include "win_system/SystemException.h"
#include <Aclapi.h>

GlobalMutex::GlobalMutex(const TCHAR* name, bool interSession, bool throwIfExist)
{
  StringStorage mutexName;

  mutexName.format(_T("%s\\%s"), interSession ? _T("Global") : _T("Local"), name);

  m_mutex = CreateMutex(0, FALSE, mutexName.getString());

  if (m_mutex == 0) {
    throw SystemException();
  }

  if (GetLastError() != ERROR_ALREADY_EXISTS) {
    setAccessToAll(m_mutex);
  } else if (throwIfExist) {
    CloseHandle(m_mutex);
    throw SystemException();
  }
}

GlobalMutex::~GlobalMutex()
{
  CloseHandle(m_mutex);
}

void GlobalMutex::lock()
{
  WaitForSingleObject(m_mutex, INFINITE);
}

void GlobalMutex::unlock()
{
  ReleaseMutex(m_mutex);
}

void GlobalMutex::setAccessToAll(HANDLE objHandle)
{
  DWORD errorCode = SetSecurityInfo(objHandle, SE_KERNEL_OBJECT,
    DACL_SECURITY_INFORMATION, // Modify DACL
    0,
    0,
    0, // Pointer to DACL (0 = access to all)
    0);
  if (errorCode != ERROR_SUCCESS) {
    throw SystemException(errorCode);
  }
}
