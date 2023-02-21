#include "ProcessHandle.h"
#include "SystemException.h"

ProcessHandle::ProcessHandle()
    :m_hProcess(0)
{
}

ProcessHandle::~ProcessHandle()
{
    if (m_hProcess != 0) {
        CloseHandle(m_hProcess);
    }
}

void ProcessHandle::openProcess(DWORD desiredAccess, BOOL inheritedHandle, DWORD processId)
{
    m_hProcess = OpenProcess(desiredAccess, inheritedHandle, processId);
    if (m_hProcess == 0) {
        StringStorage errMsg;
        errMsg.format(_T("Cant open the %d process"), dwProcessId);
        throw SystemException(errMsg.getString());
    }
}

HANDLE ProcessHandle::getHandle() const
{
    return m_hProcess;
}

void ProcessHandle::getProcessModulePath(StringStorage* exePath)
{
    TCHAR path[MAX_PATH];
    DWORD result = GetModuleFileNameEx(m_hProcess, 0, path, sizeof(path) / sizeof(TCHAR));
    if (result == 0) {
        throw SystemException(_T("Can't get process module path"));
    }
    exePath->setString(path);
}
