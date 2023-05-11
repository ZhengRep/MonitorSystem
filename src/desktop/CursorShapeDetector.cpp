#include "CursorShapeDetector.h"

const int SLEEP_TIME = 100;

CursorShapeDetector::CursorShapeDetector(UpdateKeeper* updateKeeper,
  UpdateListener* updateListener, CursorShapeGrabber* mouseGrabber,
  LocalMutex* mouseGrabLocMut, LogWriter* log)
  : UpdateDetector(updateKeeper, updateListener),
  m_mouseGrabber(mouseGrabber),
  m_mouseGrabLocMut(mouseGrabLocMut),
  m_log(log)
{
}

CursorShapeDetector::~CursorShapeDetector(void)
{
  terminate();
  wait();
}

void CursorShapeDetector::onTerminate()
{
  m_sleepTimer.notify();
}

void CursorShapeDetector::execute()
{
  m_log->info(_T("mouse shape detector thread id = %d"), getThreadId());

  while (!isTerminating()) {
    bool isCursorShapeChanged;
    {
      AutoLock al(m_mouseGrabLocMut);
      isCursorShapeChanged = m_mouseGrabber->isCursorShapeChanged();
    }
    if (isCursorShapeChanged) {
      m_updateKeeper->setCursorShapeChanged();
      doUpdate();
    }
    m_sleepTimer.waitForEvent(SLEEP_TIME);
  }
}
