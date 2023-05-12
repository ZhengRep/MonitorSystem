#include "DesktopWinImpl.h"
#include "DesktopWinImpl.h"
#include "server_config_lib/Configurator.h"
#include "desktop_ipc/UpdateHandlerClient.h"
#include "UpdateHandlerImpl.h"
#include "WindowsInputBlocker.h"
#include "desktop_ipc/UserInputClient.h"
#include "WindowsUserInput.h"
#include "win_system/Environment.h"
#include "win_system/WindowsDisplays.h"

DesktopWinImpl::DesktopWinImpl(ClipboardListener* extClipListener,
  UpdateSendingListener* extUpdSendingListener,
  AbnormDeskTermListener* extDeskTermListener,
  LogWriter* log)
  : DesktopBaseImpl(extClipListener, extUpdSendingListener, extDeskTermListener, log),
  m_wallPaper(0),
  m_deskConf(0),
  m_log(log),
  m_scrDriverFactory(Configurator::getInstance()->getServerConfig())
{
  m_log->info(_T("Creating DesktopWinImpl"));

  logDesktopInfo();

  try {
    m_updateHandler = new UpdateHandlerImpl(this, &m_scrDriverFactory, m_log);
    bool ctrlAltDelEnabled = false;
    m_userInput = new WindowsUserInput(this, ctrlAltDelEnabled, m_log);
    m_deskConf = new DesktopConfigLocal(m_log);
    applyNewConfiguration();
    m_wallPaper = new WallpaperUtil(m_log);
    m_wallPaper->updateWallpaper();

    Configurator::getInstance()->addListener(this);
  } catch (Exception& ex) {
    m_log->error(_T("exception during DesktopWinImpl creaion: %s"), ex.getMessage());
    freeResource();
    throw;
  }
  resume();
}

DesktopWinImpl::~DesktopWinImpl()
{
  m_log->info(_T("Deleting DesktopWinImpl"));
  terminate();
  wait();
  freeResource();
  m_log->info(_T("DesktopWinImpl deleted"));
}

void DesktopWinImpl::freeResource()
{
  Configurator::getInstance()->removeListener(this);

  if (m_wallPaper) delete m_wallPaper;

  if (m_updateHandler) delete m_updateHandler;
  if (m_deskConf) delete m_deskConf;
  if (m_userInput) delete m_userInput;
}

void DesktopWinImpl::onTerminate()
{
  m_newUpdateEvent.notify();
}

void DesktopWinImpl::execute()
{
  m_log->info(_T("DesktopWinImpl thread started"));

  while (!isTerminating()) {
    m_newUpdateEvent.waitForEvent();
    if (!isTerminating()) {
      m_log->debug(_T("DesktopWinImpl sendUpdate()"));
      sendUpdate();
    }
  }

  m_log->info(_T("DesktopWinImpl thread stopped"));
}

bool DesktopWinImpl::isRemoteInputTempBlocked()
{
  return !m_deskConf->isRemoteInputAllowed();
}

void DesktopWinImpl::applyNewConfiguration()
{
  m_log->info(_T("reload DesktopWinImpl configuration"));
  m_deskConf->updateByNewSettings();
}

void DesktopWinImpl::logDesktopInfo()
{
  try {
    if (Environment::isAeroOn(m_log)) {
      m_log->debug(_T("The Aero is On"));
    } else {
      m_log->debug(_T("The Aero is Off"));
    }
  } catch (Exception& e) {
    m_log->error(_T("Can't get information for the Aero: %s"), e.getMessage());
  }

  // Log all display coordinates
  WindowsDisplays m_winDisp;
  std::vector<Rect> displays = m_winDisp.getDisplays();
  m_log->debug(_T("The console desktop has %d displays"), (int)displays.size());
  for (size_t i = 0; i < displays.size(); i++) {
    m_log->debug(_T("Display %d placed at the %d, %d, %dx%d coordinates"),
      i + 1,
      displays[i].left, displays[i].top,
      displays[i].getWidth(), displays[i].getHeight());
  }
}
