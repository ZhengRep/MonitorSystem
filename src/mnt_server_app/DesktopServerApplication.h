#pragma once
#include "win_system/WindowsApplication.h"
#include "utils/AnEventListener.h"
#include "win_system/AnonymousPipe.h"
#include "desktop_ipc/BlockingGate.h"
#include "desktop_ipc/DesktopSrvDispatcher.h"
#include "desktop_ipc/UpdateHandlerServer.h"
#include "desktop_ipc/UserInputServer.h"
#include "desktop_ipc/ConfigServer.h"
#include "desktop_ipc/GateKickHandler.h"
#include "SessionChangesWatcher.h"
#include "win_system/LocalWindowsApplication.h"
#include "log_server/ClientLogger.h"
#include "log_writer/LogWriter.h"
#include "server_config_lib/ConfigReloadListener.h"
#include "utils/CommandLineArgs.h"

class DesktopServerApplication : public LocalWindowsApplication,
  public AnEventListener,
  public ConfigReloadListener
{
public:

  DesktopServerApplication(HINSTANCE appInstance, const TCHAR* windowClassName,
    const CommandLineArgs* cmdArgs);

  virtual ~DesktopServerApplication();


  virtual int run();

protected:
  virtual void onAnObjectEvent();
  virtual void onConfigReload(ServerConfig* serverConfig);

private:
  void freeResources();

  Configurator m_configurator;
  ClientLogger m_clientLogger;
  LogWriter m_log;

  // Transport
  AnonymousPipe* m_clToSrvChan;
  AnonymousPipe* m_srvToClChan;
  BlockingGate* m_clToSrvGate;
  BlockingGate* m_srvToClGate;

  DesktopSrvDispatcher* m_dispatcher;

  // Servers
  UpdateHandlerServer* m_updHandlerSrv;
  UserInputServer* m_uiSrv;
  ConfigServer* m_cfgServer;
  GateKickHandler* m_gateKickHandler;

  SessionChangesWatcher* m_sessionChangesWatcher;

  UINT m_contextSwitchResolution; // in ms
};

