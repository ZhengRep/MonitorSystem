#include "CopyRectDetector.h"

CopyRectDetector::CopyRectDetector()
{
}

CopyRectDetector::~CopyRectDetector()
{
}

void CopyRectDetector::detectWindowMovements(Rect* copyRect, Point* source)
{
  m_copyRect.clear();
  m_source.clear();

  EnumWindows((WNDENUMPROC)enumWindowsFnCopyRect, (LPARAM)this);
  m_lastWinProps = m_newWinProps;
  m_newWinProps.clear();
  *copyRect = m_copyRect;
  *source = m_source;
}

BOOL CopyRectDetector::enumWindowsFnCopyRect(HWND hwnd, LPARAM arg)
{
  CopyRectDetector* _this = (CopyRectDetector*)arg;
  return _this->checkWindowMovements(hwnd);
}

BOOL CopyRectDetector::checkWindowMovements(HWND hwnd)
{
  Rect currRect;
  if (IsWindowVisible(hwnd) && getWinRect(hwnd, &currRect)) {
    // Store window properties in the new list
    WinProp newWinProp(hwnd, &currRect);
    m_newWinProps.push_back(newWinProp);

    Rect prevRect;
    if (findPrevWinProps(hwnd, &prevRect)) {
      if ((prevRect.left != currRect.left || prevRect.top != currRect.top) &&
        currRect.area() > m_copyRect.area()) {
        m_copyRect = currRect;
        m_source.setPoint(prevRect.left, prevRect.top);

        // Adjust
        int destopX = GetSystemMetrics(SM_XVIRTUALSCREEN);
        int destopY = GetSystemMetrics(SM_YVIRTUALSCREEN);
        m_copyRect.move(-destopX, -destopY);
        m_source.move(-destopX, -destopY);
      }
    }
  }
  return TRUE;
}

bool CopyRectDetector::getWinRect(HWND hwnd, Rect* winRect)
{
  RECT rect;
  if (GetWindowRect(hwnd, &rect)) {
    winRect->fromWindowsRect(&rect);
    return true;
  }
  return false;
}

bool CopyRectDetector::findPrevWinProps(HWND hwnd, Rect* rect)
{
  std::list<WinProp>::iterator winPropsIter;
  WinProp* winProp;
  for (winPropsIter = m_lastWinProps.begin(); winPropsIter != m_lastWinProps.end();
    winPropsIter++) {
    winProp = &(*winPropsIter);
    if (winProp->hwnd == hwnd) {
      *rect = winProp->prevRect;
      return true;
    }
  }

  return false;
}
