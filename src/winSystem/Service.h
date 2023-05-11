#pragma once
#include "Utils/StringStorage.h"
#include "winSystem/SystemException.h"
#include "thread/Thread.h"

class Service
{
public:
  Service(const TCHAR* name);
  virtual ~Service();
  void run() throw(Exception);

protected:
  virtual void onStart() = 0;
  virtual void main() = 0;
  virtual void onStop() = 0;
  static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
  static void WINAPI ServiceControlHandler(DWORD dwCtrlCode);
  bool reportStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);

protected:

  StringStorage m_name;


  SERVICE_STATUS m_status;

  SERVICE_STATUS_HANDLE m_statusHandle;


  volatile bool m_isTerminating;

  static Service* g_service;
};

