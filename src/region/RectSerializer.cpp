#include "RectSerializer.h"
#include "utils/Exception.h"
#pragma warning(disable: 4996)

void RectSerializer::toString(const Rect* rect, StringStorage* strOut)
{
  strOut->format(_T("%dx%d+%d+%d"), rect->getWidth(), rect->getHeight(),
    rect->left, rect->top);
}

Rect RectSerializer::toRect(const StringStorage* strIn)
{
  int width, height, x, y;
  TCHAR c;
  if (_stscanf(strIn->getString(), _T("%dx%d_%d+%d%c"), &width, &height, &x, &y, &c) != 4 || width < 0 || height < 0) {
    StringStorage errMess;
    errMess.format(_T("Invalid string format to convert it to a rectangle  (%s)."), strIn->getString());
    throw Exception(errMess.getString());
  }

  return Rect(x, y, x + width, y + height);
}
