#include "Poller.h"

#include "region/Region.h"
#include "server_config_lib/Configurator.h"

Poller::Poller(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
  ScreenGrabber* screenGrabber, FrameBuffer* backupFrameBuffer,
  LocalMutex* frameBufferCriticalSection, LogWriter* log)
  : UpdateDetector(updateKeeper, updateListener),
  m_screenGrabber(screenGrabber),
  m_backupFrameBuffer(backupFrameBuffer),
  m_fbMutex(frameBufferCriticalSection),
  m_log(log)
{
  m_pollingRect.setRect(0, 0, 16, 16);
}

Poller::~Poller()
{
  terminate();
  wait();
}

void Poller::onTerminate()
{
  m_intervalWaiter.notify();
}

void Poller::execute()
{
  m_log->info(_T("poller thread id = %d"), getThreadId());

  FrameBuffer* screenFrameBuffer;

  {
    AutoLock al(m_fbMutex);
    screenFrameBuffer = m_screenGrabber->getScreenBuffer();
    Rect fullScreenRect(screenFrameBuffer->getDimension().getRect());
    m_updateKeeper->addChangedRect(&fullScreenRect);
  }

  while (!isTerminating()) {
    Region region;

    {
      AutoLock al(m_fbMutex);

      screenFrameBuffer = m_screenGrabber->getScreenBuffer();
      if (!screenFrameBuffer->isEqualTo(m_backupFrameBuffer)) {
        m_updateKeeper->setScreenSizeChanged();
      } else {
        m_log->info(_T("grabbing screen for polling"));
        m_screenGrabber->grab();
        m_log->info(_T("end of grabbing screen for polling"));

        // Polling
        int pollingWidth = m_pollingRect.getWidth();
        int pollingHeight = m_pollingRect.getHeight();
        int screenWidth = screenFrameBuffer->getDimension().width;
        int screenHeight = screenFrameBuffer->getDimension().height;

        Rect scanRect;
        for (int iRow = 0; iRow < screenHeight; iRow += pollingHeight) {
          for (int iCol = 0; iCol < screenWidth; iCol += pollingWidth) {
            scanRect.setRect(iCol, iRow, min(iCol + pollingWidth, screenWidth),
              min(iRow + pollingHeight, screenHeight));
            if (!screenFrameBuffer->cmpFrom(&scanRect, m_backupFrameBuffer,
              scanRect.left, scanRect.top)) {
              region.addRect(&scanRect);
            }
          }
        }

        m_updateKeeper->addChangedRegion(&region);
      }
    } // AutoLock

    // Send event
    if (!region.isEmpty()) {
      doUpdate();
    }

    unsigned int pollInterval = Configurator::getInstance()->getServerConfig()->getPollingInterval();
    m_intervalWaiter.waitForEvent(pollInterval);
  }
}
