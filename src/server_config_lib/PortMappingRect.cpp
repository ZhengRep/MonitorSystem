#include "PortMappingRect.h"
#pragma warning(disable: 4996)

PortMappingRect::PortMappingRect(int left, int top, int right, int bottom)
  :Rect(left, top, right, bottom)
{
}

PortMappingRect::PortMappingRect()
{
}

PortMappingRect::~PortMappingRect()
{
}

void PortMappingRect::toString(StringStorage* string) const
{
  string->format(_T("%dx%d+%d+%d"), right - left, bottom - top, left, top);
}

bool PortMappingRect::tryParse(const TCHAR* string)
{
  return parse(string, NULL);
}

bool PortMappingRect::parse(const TCHAR* string, PortMappingRect* out)
{
  int width, height, x, y;
  TCHAR c;
  if (_stscanf(string, _T("%dx%d+%d+%d%c"), &width, &height, &x, &y, &c) != 4) {
    return false;
  }
  if (width < 0 || height < 0) {
    return false;
  }
  if (out != NULL) {
    out->left = x;
    out->right = x + width;
    out->top = y;
    out->bottom = y + height;
  }
  return true;
}
