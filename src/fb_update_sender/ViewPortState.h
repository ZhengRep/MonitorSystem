#pragma once
#include "region/Rect.h"

class ViewPortState
{
public:
  ViewPortState();
  ViewPortState(const ViewPortState &);
  ViewPortState & operator=(const ViewPortState &viewPortState);
  ~ViewPortState();

  void setFullDesktop();
  void setArbitraryRect(const Rect* rect);
  void setPrimaryDisplay();
  void setDisplayNumber(unsigned char displayNumber);
  void setWindowHandle(HWND hwnd);
  void setWindowName(const StringStorage* windowName);
  void unresolveHwnd();
  void setProcessId(unsigned int processId);

private:
  static const int FULL_DESKTOP = 0;
  static const int PRIMARY_DISPLAY = 1;
  static const int ARBITRARY_RECT = 2;
  static const int WINDOW_RECT = 3;
  static const int DISPLAY_NUMBER = 4;
  static const int APPLICATION = 5;

  int m_mode;
  Rect m_arbitraryRect;
  HWND m_hwnd;
  StringStorage m_windowName;
  bool m_windowIsResolved;
  unsigned char m_displayNumber;
  unsigned int m_processId;

  friend class ViewPort;
};

