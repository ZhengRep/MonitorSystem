#pragma once
#include "Utils/CommonHeader.h"
#include "SystemException.h"

class Process
{
public:
  Process(const TCHAR* path = _T(""), const TCHAR* args = _T(""));
  virtual ~Process();
  void setFilename(const TCHAR* path);
  void setArguments(const TCHAR* args);
  void setStandardIoHandles(HANDLE stdIn, HANDLE stdOut, HANDLE stdErr);
  void setHandleInheritances(bool handlesIsInherited);
  virtual void start() throw(SystemException);
  virtual void kill() throw(SystemException);
  void waitForExit();
  void stopWait();
  DWORD getExitCode() throw(SystemException);
  HANDLE getProcessHandle();

protected:
  StringStorage getCommandLineString();
  void getStartupInfo(STARTUPINFO* sti);
  void cleanup();

  StringStorage m_path;
  StringStorage m_args;

  HANDLE m_hProcess;
  HANDLE m_hThread;
  HANDLE m_hStopWait;

  HANDLE m_stdIn;
  HANDLE m_stdOut;
  HANDLE m_stdErr;

  bool m_handlesIsInherited;
};

