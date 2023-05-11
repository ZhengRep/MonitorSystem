#include "WallpaperUtil.h"
#include "server_config_lib/Configurator.h"
#include "winSystem/Impersonator.h"
#include "winSystem/AutoImpersonator.h"

WallpaperUtil::WallpaperUtil(LogWriter* log)
  : m_wasDisabled(false),
  m_log(log)
{
  Configurator::getInstance()->addListener(this);
}

WallpaperUtil::~WallpaperUtil()
{
  Configurator::getInstance()->removeListener(this);
  if (m_wasDisabled) {
    try {
      restoreWallpaper();
      m_log->info(_T("Wallpaper was successfully restored"));
    } catch (Exception& e) {
      m_log->error(e.getMessage());
    }
  }
}

void WallpaperUtil::updateWallpaper()
{
  try {
    ServerConfig* srvConf = Configurator::getInstance()->getServerConfig();
    if (srvConf->isRemovingDesktopWallpaperEnabled()) {
      disableWallpaper();
      m_wasDisabled = true;
      m_log->info(_T("Wallpaper was successfully disabled"));
    } else {
      if (m_wasDisabled) {
        restoreWallpaper();
        m_log->info(_T("Wallpaper was successfully restored"));
        m_wasDisabled = false;
      }
    }
  } catch (Exception& e) {
    m_log->error(e.getMessage());
  }
}

void WallpaperUtil::onConfigReload(ServerConfig* serverConfig)
{
  updateWallpaper();
}

void WallpaperUtil::disableWallpaper()
{
  m_log->info(_T("Try to disable wallpaper"));
  Impersonator imp(m_log);
  AutoImpersonator ai(&imp, m_log);
  TCHAR path[MAX_PATH] = _T("");

  if (SystemParametersInfo(SPI_GETDESKWALLPAPER, MAX_PATH, path, 0) == 0) {
    path[0] = '\0';
  }

  if (SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, NULL, 0) == 0) {
    throw SystemException(_T("Cannot disable desktop wallpaper"));
  }
  m_wallparerPath = StringStorage(path);
}

void WallpaperUtil::restoreWallpaper()
{
  m_log->info(_T("Try to restore wallpaper"));
  Impersonator imp(m_log);
  AutoImpersonator ai(&imp, m_log);
  int result;

  if (m_wallparerPath.getLength() == 0) {
    result = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, 0, 0);
  } else {
    result = SystemParametersInfo(SPI_SETDESKWALLPAPER, m_wallparerPath.getSize(), (void*)m_wallparerPath.getString(), 0);
  }

  if (result == 0) {
    throw SystemException(_T("Cannot restore desktop wallpaper"));
  }
}
