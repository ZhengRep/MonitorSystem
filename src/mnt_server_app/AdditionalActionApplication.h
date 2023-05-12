#pragma once
#include "utils/CommonHeader.h"
#include "win_system/LocalWindowsApplication.h"

class AdditionalActionApplication : public LocalWindowsApplication
{
public:
  static const TCHAR LOCK_WORKSTATION_KEY[];
  static const TCHAR LOGOUT_KEY[];

  AdditionalActionApplication(HINSTANCE hInstance,
    const TCHAR* windowClassName, const TCHAR* commandLine);
  virtual ~AdditionalActionApplication();
  virtual int run();

private:
  StringStorage m_commandLine;
};

