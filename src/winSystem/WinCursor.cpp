#include "WinCursor.h"

WinCursor::WinCursor()
{
}

WinCursor::~WinCursor()
{
}

Point WinCursor::getCursorPos() const
{
  POINT curPoint;
  GetCursorPos(&curPoint);
  // Correcting point to frame buffer coordinates
  curPoint.x -= GetSystemMetrics(SM_XVIRTUALSCREEN);
  curPoint.y -= GetSystemMetrics(SM_YVIRTUALSCREEN);
  return Point(curPoint.x, curPoint.y);
}
