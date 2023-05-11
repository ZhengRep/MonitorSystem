#pragma once
#include "Utils/CommonHeader.h"

extern "C"
{
  __declspec(dllexport) bool setHook(HWND targetWinHwnd);
  __declspec(dllexport) bool unsetHook();
}

