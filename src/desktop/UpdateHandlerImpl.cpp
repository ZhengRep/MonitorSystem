#include "UpdateHandlerImpl.h"
#include "utils/Macros.h"

UpdateHandlerImpl::UpdateHandlerImpl(UpdateListener* externalUpdateListener, ScreenDriverFactory* scrDriverFactory,
  LogWriter* log)
  : m_externalUpdateListener(externalUpdateListener),
  m_fullUpdateRequested(false),
  m_log(log)
{
  m_screenDriver = scrDriverFactory->createScreenDriver(&m_updateKeeper,
    this,
    &m_backupFrameBuffer,
    &m_fbLocMut, log);
  // At this point the screen driver must contain valid screen properties.
  m_backupFrameBuffer.assignProperties(m_screenDriver->getScreenBuffer());
  m_updateKeeper.setBorderRect(&unmove(m_screenDriver->getScreenDimension().getRect()));
  m_updateFilter = new UpdateFilter(m_screenDriver,
    &m_backupFrameBuffer,
    &m_fbLocMut, log);

  // At this point all common resources will be covered the mutex for changes.
  m_screenDriver->executeDetection();

  // Force first update with full screen grab
  m_absoluteRect = m_backupFrameBuffer.getDimension().getRect();
  m_updateKeeper.addChangedRect(&m_absoluteRect);
  doUpdate();
}

UpdateHandlerImpl::~UpdateHandlerImpl()
{
  m_screenDriver->terminateDetection();
  delete m_updateFilter;
  delete m_screenDriver;
}

void UpdateHandlerImpl::extract(UpdateContainer* updateContainer)
{
  Rect copyRect;
  Point copySrc;
  m_log->debug(_T("UpdateHandlerImpl: getCopiedRegion"));
  m_screenDriver->getCopiedRegion(&copyRect, &copySrc);
  {
    AutoLock al(&m_updateKeeper); 
    m_updateKeeper.addCopyRect(&copyRect, &copySrc);
    m_log->debug(_T("UpdateHandlerImpl: extract Copy Rect"));
    m_updateKeeper.extract(updateContainer);
  }

  m_log->debug(_T("UpdateHandlerImpl: getVideoRegion"));
  updateContainer->videoRegion = m_screenDriver->getVideoRegion();
  // Constrain the video region to the current frame buffer border.
  m_log->debug(_T("UpdateHandlerImpl: getRect"));
  Region fbRect(getFrameBufferDimension().getRect());
  m_log->debug(_T("UpdateHandlerImpl: intersect"));
  updateContainer->videoRegion.intersect(&fbRect);

  m_log->debug(_T("UpdateHandlerImpl::extract : filter updates"));
  m_updateFilter->filter(updateContainer);

  if (!m_absoluteRect.isEmpty()) {
    updateContainer->changedRegion.addRect(&unmove(m_screenDriver->getScreenBuffer()->getDimension().getRect()));
    m_absoluteRect.clear();
  }

  // Checking for screen properties changing or frame buffers differ
  m_log->debug(_T("UpdateHandlerImpl::extract : Checking for screen properties changing or frame buffers differ"));
  if (m_screenDriver->getScreenPropertiesChanged() ||
    !m_backupFrameBuffer.isEqualTo(m_screenDriver->getScreenBuffer())) {
    Dimension currentDimension = m_backupFrameBuffer.getDimension();
    Dimension newDimension = m_screenDriver->getScreenDimension();
    if (m_screenDriver->getScreenSizeChanged() || !currentDimension.isEqualTo(&newDimension)) {
      updateContainer->screenSizeChanged = true;
    }
    m_log->debug(_T("UpdateHandlerImpl::extract : applyNewScreenProperties()"));
    applyNewScreenProperties();
    {
      AutoLock al(&m_fbLocMut);
      m_backupFrameBuffer.clone(m_screenDriver->getScreenBuffer());
    }
    updateContainer->changedRegion.clear();
    updateContainer->copiedRegion.clear();
    m_absoluteRect = m_backupFrameBuffer.getDimension().getRect();
    m_updateKeeper.setBorderRect(&m_absoluteRect);
  }

  // Cursor position must always be present.
  updateContainer->cursorPos = m_screenDriver->getCursorPosition();
  {
    int x = updateContainer->cursorPos.x;
    int y = updateContainer->cursorPos.y;
    m_log->debug(_T("UpdateHandlerImpl::extract : update cursor position (%d,%d)"), x, y);
  }
  // Checking for mouse shape changing
  if (updateContainer->cursorShapeChanged || m_fullUpdateRequested) {
    // Update cursor shape
    m_screenDriver->grabCursorShape(&unmove(m_backupFrameBuffer.getPixelFormat()));
    // Store cursor shape
    m_cursorShape.clone(m_screenDriver->getCursorShape());

    m_fullUpdateRequested = false;
  }
  m_log->debug(_T("UpdateHandlerImpl::extract finished"));
}

void UpdateHandlerImpl::applyNewScreenProperties()
{
  int applyTryCount = 3;
  bool applyResult = true;
  do {
    if (!applyResult) {
      WindowsEvent waitEvent; // No way to made it abortable.
      waitEvent.waitForEvent(1000);
    }
    m_log->info(_T("Screen properties changed, applying new screen properties, total tries = %d"), applyTryCount);
    applyResult = m_screenDriver->applyNewScreenProperties();
    applyTryCount--;
  } while (applyTryCount > 0 && !applyResult);
  if (!applyResult) {
    throw Exception(_T("Unable set new screen properties at all tries"));
  }
}

void UpdateHandlerImpl::setFullUpdateRequested(const Region* region)
{
  m_updateKeeper.addChangedRegion(region);
  m_fullUpdateRequested = true;
}

void UpdateHandlerImpl::executeDetectors()
{
  // FIXME: Why following string is here?
  m_backupFrameBuffer.assignProperties(m_screenDriver->getScreenBuffer());

  m_screenDriver->executeDetection();
}

void UpdateHandlerImpl::terminateDetectors()
{
  m_screenDriver->terminateDetection();
}

void UpdateHandlerImpl::onUpdate()
{
  UpdateContainer updCont;
  m_updateKeeper.getUpdateContainer(&updCont);
  if (!updCont.isEmpty()) {
    doUpdate();
  }
}

bool UpdateHandlerImpl::checkForUpdates(Region* region)
{
  return m_updateKeeper.checkForUpdates(region);
}

void UpdateHandlerImpl::setExcludedRegion(const Region* excludedRegion)
{
  m_updateKeeper.setExcludedRegion(excludedRegion);
}
