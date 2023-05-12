#pragma once
#include "region/Point.h"
#include "utils/CommonHeader.h"

class WinCursor
{
public:
  WinCursor();
  virtual ~WinCursor();

  Point getCursorPos() const;
};

