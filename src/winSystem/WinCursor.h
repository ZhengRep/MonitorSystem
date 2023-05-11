#pragma once
#include "region/Point.h"
#include "Utils/CommonHeader.h"

class WinCursor
{
public:
  WinCursor();
  virtual ~WinCursor();

  Point getCursorPos() const;
};

