#pragma once
#include "region/Rect.h"
#include "utils/DateTime.h"
#include <vector>
#include "thread/LocalMutex.h"

class WindowsDisplays
{
public:
  WindowsDisplays();
  virtual ~WindowsDisplays();

  void getDisplayCoordinates(unsigned char displayNumber, Rect* rect);
  std::vector<Rect> getDisplays();

private:
  void update();
  static BOOL CALLBACK monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
  bool isAlreadyUpdated();

  int m_xVirtualScreen;
  int m_yVirtualScreen;
  std::vector<Rect> m_displayRects;
  LocalMutex m_displayRectsMutex;

  static const unsigned int UPDATE_INTERVAL = 3000;
  DateTime m_latestUpdateTime;
};

