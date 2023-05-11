#include "CursorPositionDetector.h"

const int MOUSE_SLEEP_TIME = 10;

CursorPositionDetector::CursorPositionDetector(UpdateKeeper* updateKeeper,
  UpdateListener* updateListener, LogWriter* log)
  : UpdateDetector(updateKeeper, updateListener),
  m_log(log)
{
}

CursorPositionDetector::~CursorPositionDetector(void)
{
  terminate();
  wait();
}

Point CursorPositionDetector::getCursorPos()
{
  return m_cursor.getCursorPos();
}

void CursorPositionDetector::onTerminate()
{
  m_sleepTimer.notify();
}

void CursorPositionDetector::execute()
{
  m_log->info(_T("mouse detector thread id = %d"), getThreadId());

  Point curPoint;

  while (!isTerminating()) {
    curPoint = m_cursor.getCursorPos();
    if (!m_lastCursorPos.isEqualTo(&curPoint)) {
      m_lastCursorPos = curPoint;
      m_updateKeeper->setCursorPosChanged(&m_lastCursorPos);
      doUpdate();
    }
    m_sleepTimer.waitForEvent(MOUSE_SLEEP_TIME);
  }
}
