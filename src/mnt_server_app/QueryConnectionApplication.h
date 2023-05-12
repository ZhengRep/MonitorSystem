#pragma once
#include "win_system/LocalWindowsApplication.h"

class QueryConnectionApplication: public LocalWindowsApplication
{
public:
  QueryConnectionApplication(HINSTANCE hInstance,
    const TCHAR* windowClassName, const TCHAR* cmdLine);
  virtual ~QueryConnectionApplication();
  virtual int run();
  static int execute(const TCHAR* peerAddr, bool acceptByDefault, DWORD timeOutSec) throw(Exception);

private:
  StringStorage m_cmdLine;
};

