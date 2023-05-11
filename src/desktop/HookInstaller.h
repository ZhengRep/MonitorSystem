#pragma once
#include "Utils/Command.h"
#include "winSystem/DynamicLibrary.h"

class HookInstaller
{
public:
  HookInstaller();
  ~HookInstaller();

  // Instals hooks to windowed applications.
  // targedWinHwnd - is hwnd to a target window that will receive
  // hooked messages.
  void install(HWND targedWinHwnd);
  // Uninstals hooks from windowed applications.
  void uninstall();

private:
  static const TCHAR LIBRARY_NAME[];
  static const char SET_HOOK_FUNCTION_NAME[];
  static const char UNSET_HOOK_FUNCTION_NAME[];

  DynamicLibrary m_lib;
  FARPROC m_pSetHook;
  FARPROC m_pUnSetHook;
};

