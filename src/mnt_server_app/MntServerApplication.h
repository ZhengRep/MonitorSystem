#pragma once
#include "utils/CommonHeader.h"
#include "win_system/WindowsApplication.h"
#include "MntServer.h"
#include "MntServerListener.h"
#include "WsConfigRunner.h"
#include "log_writer/FileLogger.h"
#include "LogInitListener.h"


class MntServerApplication : public WindowsApplication,
  public MntServerListener,
  private LogInitListener
{
public:
  
  MntServerApplication(HINSTANCE hInstance,
    const TCHAR* windowClassName,
    const TCHAR* commandLine,
    NewConnectionEvents* newConnectionEvents);
  
  virtual ~MntServerApplication();

  /**
   * Runs MNT server windows application.
   *
   * Makes several things:
   *   1) Starts MNT server.
   *   2) Starts MNT Control application.
   *   3) Enters main windows message loop.
   *   4) Stops MNT server.
   */
  virtual int run();

  virtual void onMntServerShutdown();

private:
  virtual void onLogInit(const TCHAR* logDir, const TCHAR* fileName, unsigned char logLevel);

  virtual void onChangeLogProps(const TCHAR* newLogDir, unsigned char newLevel);

  FileLogger m_fileLogger;

  StringStorage m_commandLine;
 
  MntServer* m_mntServer;
 
  WsConfigRunner* m_mntControlRunner;

  NewConnectionEvents* m_newConnectionEvents;
};

