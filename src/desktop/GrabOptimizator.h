#pragma once
#include "ScreenDriver.h"
#include "region/Region.h"
#include "utils/DemandTimer.h"
#include "log_writer/LogWriter.h"
#include <vector>
#include <list>

class GrabOptimizator
{
public:
  GrabOptimizator(LogWriter* log);
  ~GrabOptimizator();

  bool grab(const Region* grabRegion, ScreenDriver* grabber);

private:
  bool getWholeTCompleted();
  bool getGCompleted();
  bool getOptimizationAvailable(ScreenDriver* grabber);
  bool isAvailableWholeStats();
  bool isAvailableFragmentStats();

  void refreshStatistic(ScreenDriver* grabber);
  int getArea(const std::vector<Rect>* rects);
  bool isAlikeToWhole(const std::vector<Rect>* rects);
  bool isEnoughForWholeStats(const Rect* rect);
  bool isAlikeToFragments(const std::vector<Rect>* rects);
  void logStatistic();

  static const size_t MIN_ELEMENTS_SIZE = 3;
  static const size_t MAX_ELEMENTS_SIZE = 10;

  __int64 grabWhole(ScreenDriver* grabber);
  __int64 grabOneRect(const Rect* rect, ScreenDriver* grabber);
  __int64 grabFragments(const std::vector<Rect>* rects, ScreenDriver* grabber);

  void addWholeTElement(double wholeT);
  void removeObsoleteWholeTElements();
  void removeFirstWholeTElement();

  void addFragmentStats(double g);
  void removeObsoleteFragmentStats();
  void removeFirstElementsFromFragmentStats();

  int m_wholeS;
  std::list<double> m_wholeTElements;
  double m_wholeTSum;

  std::list<double> m_gElements;
  double m_gSum;

  DemandTimer m_timer;
  LogWriter* m_log;
};

