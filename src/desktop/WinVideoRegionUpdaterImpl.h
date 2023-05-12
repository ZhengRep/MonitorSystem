#pragma once
#include "utils/DateTime.h"
#include "region/Region.h"
#include "utils/StringVector.h"
#include "ScreenDriver.h"
#include "thread/LocalMutex.h"
#include "log_writer/LogWriter.h"
#include "thread/Thread.h"
#include "win_system/WindowsEvent.h"

class WinVideoRegionUpdaterImpl : public ScreenDriver, Thread
{
public:
  WinVideoRegionUpdaterImpl(LogWriter* log);
  virtual ~WinVideoRegionUpdaterImpl();
protected:
  virtual void execute();
  virtual void onTerminate();
private:
  virtual Region getVideoRegion();
  void updateVideoRegion();
  void getClassNamesAndRectsFromConfig(StringVector& classNames, std::vector<Rect>& rects);
  unsigned int  getInterval();
  Region getRectsByClass(StringVector classNames);
  Region getRectsByCoords(std::vector<Rect>& rects);

  DateTime m_lastVidUpdTime;
  Region m_vidRegion;
  LocalMutex m_regionMutex;
  LogWriter* m_log;
  WindowsEvent m_sleeper;
};

