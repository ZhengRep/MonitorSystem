#pragma once
#include "Utils/CommonHeader.h"

class Keymap
{
public:
  bool keySymToVirtualCode(UINT32 keySym, BYTE* vkCode, bool* extended);
  bool virtualCodeToKeySym(UINT32* keySym, BYTE vkCode);
  bool keySymToUnicodeChar(UINT32 keySym, WCHAR* ch);
  bool unicodeCharToKeySym(WCHAR ch, UINT32* keySym);
};

