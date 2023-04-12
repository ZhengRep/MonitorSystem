#pragma once
#include "Utils/CommonHeader.h"
#include "Rect.h"

class RectSerializer
{
public:
  static void toString(const Rect* rect, StringStorage* strOut);
  static Rect toRect(const StringStorage* strIn);
};

