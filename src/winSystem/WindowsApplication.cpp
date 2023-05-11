#include "WindowsApplication.h"
#include "thread/AutoLock.h"

LocalMutex WindowsApplication::m_MDLMutex;
list<HWND> WindowsApplication::m_modelessDialogList;

WindowsApplication::WindowsApplication(HINSTANCE appInstance, const TCHAR* windowClassName)
  : m_appInstance(appInstance),
    m_mainWindow(0),
    m_windowClassName(windowClassName)
{
}

WindowsApplication::~WindowsApplication()
{
}

int WindowsApplication::run()
{
  WNDCLASS wndClass;
  registerWindowClass(&wndClass);
  createWindow(wndClass.lpszClassName);
  try {
    return processMessages();
  } catch (...) {
    return 1;
  }
}

void WindowsApplication::shutdown()
{
  PostMessage(m_mainWindow, WM_CLOSE, 0, 0);
}

void WindowsApplication::postMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
  PostMessage(m_mainWindow, message, wParam, lParam);
}

void WindowsApplication::addModelessDialog(HWND dialogWindow)
{
  AutoLock l(&m_MDLMutex);

  m_modelessDialogList.push_back(dialogWindow);
}

void WindowsApplication::removeModelessDialog(HWND dialogWindow)
{
  AutoLock l(&m_MDLMutex);

  m_modelessDialogList.remove(dialogWindow);
}

void WindowsApplication::createWindow(const TCHAR* className)
{
  m_mainWindow = CreateWindow(className,
    0, 0,
    0, 0, 0, 0,
    HWND_MESSAGE, 0,
    m_appInstance,
    0);
}

void WindowsApplication::registerWindowClass(WNDCLASS* wndClass)
{
  memset(wndClass, 0, sizeof(WNDCLASS));

  // Set default values. Derived classes can redefine this fields
  wndClass->lpfnWndProc = wndProc;
  wndClass->hInstance = m_appInstance;
  wndClass->lpszClassName = m_windowClassName.getString();

  RegisterClass(wndClass);
}

int WindowsApplication::processMessages()
{
  MSG msg;
  BOOL ret;
  while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0) {
    if (ret < 0) {
      return 1;
    }
    if (!processDialogMessage(&msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  return (int)msg.wParam;
}

LRESULT WindowsApplication::wndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  case WM_CLOSE:
    DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, msg, wparam, lparam);
}

bool WindowsApplication::processDialogMessage(MSG* msg)
{
  AutoLock l(&m_MDLMutex);
  for (list<HWND>::iterator iter = m_modelessDialogList.begin();
    iter != m_modelessDialogList.end(); iter++) {
    if (IsDialogMessage(*iter, msg)) {
      return true;
    }
  }
  return false;
}
