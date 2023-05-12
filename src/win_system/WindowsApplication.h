#pragma once
#include "utils/winhdr.h"
#include "thread/LocalMutex.h"
#include <list>
using namespace std;

/*
Base WindowsApplication instance
Have hidden main window and main message loop
*/
class WindowsApplication
{
public:
  WindowsApplication(HINSTANCE appInstance, const TCHAR* windowClassName);
  virtual ~WindowsApplication();
  virtual int run();
  virtual void shutdown();
  virtual void postMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
  static void addModelessDialog(HWND dialogWindow);
  static void removeModelessDialog(HWND dialogWindow);

protected:
  virtual void createWindow(const TCHAR* className);
  virtual void registerWindowClass(WNDCLASS* wndClass);
  virtual int processMessages();
  static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

  HINSTANCE m_appInstance;
  HWND m_mainWindow;
  StringStorage m_windowClassName;

protected:
  static bool processDialogMessage(MSG* msg);

private:
  static LocalMutex m_MDLMutex; //Modeless dialog list mutex
  static list<HWND> m_modelessDialogList;

};

