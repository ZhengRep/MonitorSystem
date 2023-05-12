#pragma once
#include "WindowsApplication.h"
#include "SystemException.h"

class LocalWindowsApplication: public WindowsApplication
{
public:
  /*
  Create class instance and prepare environment for appliction
  (selects needed window station and desktop)
  */
  LocalWindowsApplication(HINSTANCE hInstance, const TCHAR* windowClassName);
  virtual ~LocalWindowsApplication();
};

