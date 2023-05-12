#pragma once
#include "utils/inttypes.h"

class ClientInputEventListener
{
public:
  virtual void onKeyboardEvent(UINT32 keySym, bool down) = 0;
  virtual void onMouseEvent(UINT16 x, UINT16 y, UINT8 buttonMask) = 0;
};