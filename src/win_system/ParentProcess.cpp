#include "ParentProcess.h"

ParentProcess::ParentProcess()
  :m_isInitialized(false)
{
  init();
}

ParentProcess::~ParentProcess()
{
}

bool ParentProcess::isAlive()
{
  bool result = false;
  if (m_isInitialized) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
      PROCESSENTRY32 currParentPe;
      DWORD parentId = m_parentPe.th32ProcessID;
      result = getProcessEntry(&currParentPe, parentId, hSnapshot);
      CloseHandle(hSnapshot);
    }
  }
  return result;
}

void ParentProcess::init()
{
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnapshot != INVALID_HANDLE_VALUE) {
    PROCESSENTRY32 selfPe;
    selfPe.dwSize = sizeof(PROCESSENTRY32);
    DWORD selfId = GetCurrentProcessId();
    // Find the self process entry to get parent process id.
    if (getProcessEntry(&selfPe, selfId, hSnapshot)) {
      // Find the parent process entry
      DWORD parentId = selfPe.th32ParentProcessID;
      m_isInitialized = getProcessEntry(&m_parentPe, parentId, hSnapshot);
    }
    CloseHandle(hSnapshot);
  }
}

bool ParentProcess::getProcessEntry(PROCESSENTRY32* peOut, DWORD pid, HANDLE hSnapshot)
{
  peOut->dwSize = sizeof(PROCESSENTRY32);

  bool result = false;
  if (Process32First(hSnapshot, peOut) == 0) {
    return false;
  }
  do {
    if (peOut->th32ProcessID == pid) {
      result = true;
    }
  } while (!result && Process32Next(hSnapshot, peOut) != 0);
  return result;
}
