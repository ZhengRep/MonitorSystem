#pragma once
#include "utils/winhdr.h"
#include "SystemException.h"

class Keyboard
{
public:
  static void getState(BYTE state[256]) throw(SystemException);
  static void setState(BYTE state[256]) throw(SystemException);
  static bool isKeyPressed(BYTE vkCode);
};

