#pragma once
#include "Utils/CommonHeader.h"
#include "winSystem/LocalWindowsApplication.h"

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

