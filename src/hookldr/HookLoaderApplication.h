#pragma once
#include "win_system/LocalWindowsApplication.h"
#include "ParentWatcher.h"

class HookLoaderApplication: public WindowsApplication, private ParentEventsListener
{
public:
  HookLoaderApplication(HINSTANCE appInstance, const TCHAR* windowClassName, const TCHAR* commandLine);
  virtual ~HookLoaderApplication();

private:
  virtual void onParentTerminate();

  virtual void registerWindowClass(WNDCLASS* wndClass);
  virtual int processMessages();

  static LRESULT CALLBACK hookLoaderWndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

  StringStorage m_commandLine;

  ParentWatcher* m_parentWatcher;
};

