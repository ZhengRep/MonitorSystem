#include "HookLoaderApplication.h"
#include "utils/StringParser.h"
#include "mnt_server_app/NamingDefs.h"
#include "desktop/HookInstaller.h"

HookLoaderApplication::HookLoaderApplication(HINSTANCE appInstance, const TCHAR* windowClassName, const TCHAR* commandLine)
  : WindowsApplication(appInstance, windowClassName),
    m_commandLine(commandLine)
{
  m_parentWatcher = new ParentWatcher(this);
}

HookLoaderApplication::~HookLoaderApplication()
{
  delete m_parentWatcher;
}

void HookLoaderApplication::onParentTerminate()
{
  shutdown();
}

void HookLoaderApplication::registerWindowClass(WNDCLASS* wndClass)
{
  memset(wndClass, 0, sizeof(WNDCLASS));

  // Set default values. Derived classes can redefine this fields
  wndClass->lpfnWndProc = hookLoaderWndProc;
  wndClass->hInstance = m_appInstance;
  wndClass->lpszClassName = HookDefinitions::HOOK_LOADER_WINDOW_CLASS;

  RegisterClass(wndClass);
}

int HookLoaderApplication::processMessages()
{
  // m_commandLine - is a target window hwnd by 64-bit text number.
  UINT64 targetWinValue = 0;
  HWND targetWinHwnd = 0;
  if (!StringParser::parseUInt64(m_commandLine.getString(), &targetWinValue)) {
    throw Exception(_T("HookLoaderApplication failed: Can't parse the command line to UINT64"));
  }
  targetWinHwnd = (HWND)targetWinValue;

  HookInstaller hookInstaller;
  hookInstaller.install(targetWinHwnd);

  MSG msg;
  BOOL ret;
  while (ret = GetMessage(&msg, NULL, 0, 0) != 0) {
    if (ret < 0) {
      return 1;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

LRESULT HookLoaderApplication::hookLoaderWndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  if (msg == HookDefinitions::LOADER_CLOSE_CODE) {
    PostMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  }

  switch (msg) {
  case WM_CLOSE:
    DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, msg, wparam, lparam);
}
