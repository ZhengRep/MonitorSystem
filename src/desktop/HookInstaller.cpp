#include "HookInstaller.h"

#ifndef _WIN64 
const TCHAR HookInstaller::LIBRARY_NAME[] = _T("screenhook32.dll");
#else
const TCHAR HookInstaller::LIBRARY_NAME[] = _T("screenhook64.dll");
#endif

const char HookInstaller::SET_HOOK_FUNCTION_NAME[] = "setHook";
const char HookInstaller::UNSET_HOOK_FUNCTION_NAME[] = "unsetHook";

typedef bool (*SetHookFunction)(HWND targedWinHwnd);
typedef bool (*UnsetHookFunction)();

HookInstaller::HookInstaller()
  : m_lib(LIBRARY_NAME),
    m_pSetHook(0),
    m_pUnSetHook(0)
{
}

HookInstaller::~HookInstaller()
{
  uninstall();
}

void HookInstaller::install(HWND targedWinHwnd)
{
  HINSTANCE hinst = GetModuleHandle(0);

  m_pSetHook = m_lib.getProcAddress(SET_HOOK_FUNCTION_NAME);
  m_pUnSetHook = m_lib.getProcAddress(UNSET_HOOK_FUNCTION_NAME);
  if (!m_pSetHook || !m_pUnSetHook) {
    throw Exception(_T("Cannot find the setHook() and unsetHook() functions"));
  }

  //Hooks initializing
  SetHookFunction setHookFunction = (SetHookFunction)m_pSetHook;
  if (!setHookFunction(targedWinHwnd)) {
    throw Exception(_T("setHook() function failed"));
  }
}

void HookInstaller::uninstall()
{
  if (m_pUnSetHook) {
    UnsetHookFunction unsetHookFunction = (UnsetHookFunction)m_pUnSetHook;
    if (!unsetHookFunction()) {
      throw Exception(_T("unsetHook() function failed"));
    }
    m_pUnSetHook = 0;
    m_pSetHook = 0;
  }
}
