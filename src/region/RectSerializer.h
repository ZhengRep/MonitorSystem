#pragma once
#include "utils/CommonHeader.h"
#include "Rect.h"

class RectSerializer
{
public:
  static void toString(const Rect* rect, StringStorage* strOut);
  static Rect toRect(const StringStorage* strIn);
};

