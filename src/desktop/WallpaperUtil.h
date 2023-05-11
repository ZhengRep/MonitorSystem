#pragma once
#include "log_writer/LogWriter.h"
#include "server_config_lib/ConfigReloadListener.h"

class WallpaperUtil: protected ConfigReloadListener
{
public:
  WallpaperUtil(LogWriter* log);
  virtual ~WallpaperUtil();

  virtual void updateWallpaper();

protected:
  virtual void onConfigReload(ServerConfig* serverConfig);

private:
  void disableWallpaper();
  void restoreWallpaper();
  LogWriter* m_log;
  StringStorage m_wallparerPath;
  bool m_wasDisabled;
};

