#pragma once
#include "ViewPortState.h"
#include "rfb/FrameBuffer.h"
#include "desktop/Desktop.h"
#include "thread/LocalMutex.h"
#include "Utils/DateTime.h"
#include "log_writer/LogWriter.h"

class ViewPort
{
  ViewPort(LogWriter* log);
  ViewPort(const ViewPortState* viewPortState, LogWriter* log);
  ~ViewPort();

  void initDesktopInterface(Desktop* desktop);
  void update(const Dimension* fbDimension);
  Rect getViewPortRect();
  bool getOnlyApplication();
  unsigned int getApplicationId();
  void getApplicationRegion(Region* region);
  void changedState(const ViewPortState* newState);

private:
  ViewPort(const ViewPort&); //Can't used outside
  ViewPort& operator =(const ViewPort&);
  void resolveWindowName();

  static const int RESOLVING_PERIOD = 3000;
  Desktop* m_desktop;
  ViewPortState m_state;
  Rect m_rect;
  Region m_appRegion;
  LocalMutex m_stateMutex;
  DateTime m_latestHwndResolveingTime;
  LogWriter* m_log;
};

