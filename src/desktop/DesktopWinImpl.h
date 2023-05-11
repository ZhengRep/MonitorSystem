#pragma once
#include "Utils/CommonHeader.h"
#include "DesktopConfigLocal.h"
#include "thread/GuiThread.h"
#include "WallpaperUtil.h"
#include "DesktopBaseImpl.h"
#include "Win32ScreenDriverFactory.h"
#include "log_writer/LogWriter.h"

class DesktopWinImpl : public GuiThread, public DesktopBaseImpl
{
public:
  DesktopWinImpl(ClipboardListener* extClipListener, UpdateSendingListener* extUpdSendingListener,
    AbnormDeskTermListener* extDeskTermListener, LogWriter* log);
  virtual ~DesktopWinImpl();

protected:
  virtual void execute();
  virtual void onTerminate();

private:
  void freeResource();

  void logDesktopInfo();

  virtual bool isRemoteInputTempBlocked();
  virtual void applyNewConfiguration();

  Win32ScreenDriverFactory m_scrDriverFactory;
  WallpaperUtil* m_wallPaper;
  DesktopConfigLocal* m_deskConf;
  LogWriter* m_log;
};

