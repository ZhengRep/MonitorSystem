#pragma once
#include "utils/CommonHeader.h"

extern "C"
{
  __declspec(dllexport) bool setHook(HWND targetWinHwnd);
  __declspec(dllexport) bool unsetHook();
}

