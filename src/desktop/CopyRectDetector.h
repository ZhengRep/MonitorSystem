#pragma once
#include "region/Rect.h"
#include "region/Point.h"
#include "Utils/CommonHeader.h"
#include <list>

struct WinProp
{
  WinProp(HWND _hwnd, Rect* _prevRect) { hwnd = _hwnd; prevRect = *_prevRect; }
  HWND hwnd;
  Rect prevRect;
};

class CopyRectDetector
{
public:
  CopyRectDetector();
  virtual ~CopyRectDetector();

  void detectWindowMovements(Rect* copyRect, Point* source);

protected:
  static BOOL CALLBACK enumWindowsFnCopyRect(HWND hwnd, LPARAM arg);
  BOOL checkWindowMovements(HWND hwnd);

  bool getWinRect(HWND hwnd, Rect* winRect);

  // If window properties successfully was found then function returns
  // true. Else this function returns false.
  bool findPrevWinProps(HWND hwnd, Rect* rect);

  Rect m_copyRect;
  Point m_source;

  std::list<WinProp> m_lastWinProps;
  std::list<WinProp> m_newWinProps;
};

