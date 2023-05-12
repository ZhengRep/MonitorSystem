#pragma once
#include "utils/CommonHeader.h"
#include "desktop/WinServiceDesktopFactory.h"
#include "desktop/ApplicationDesktopFactory.h"
#include "RfbClientManager.h"
#include "RfbServer.h"
#include "ControlServer.h"
#include "MntServerListener.h"
#include "thread/ZombieKiller.h"
#include "thread/LocalMutex.h"
#include "log_writer/LogWriter.h"
#include "utils/Singleton.h"
#include "NewConnectionEvents.h"
#include "utils/ListenerContainer.h"
#include "server_config_lib/Configurator.h"
#include "mnt_control_app/MntServerInfo.h"
#include "LogInitListener.h"

class MntServer : public Singleton<MntServer>,
  public ListenerContainer<MntServerListener*>,
  public ConfigReloadListener,
  public RfbClientManagerEventListener
{
public:
  MntServer(bool runsInServiceContext, NewConnectionEvents* newConnectionEvents,
    LogInitListener* logInitListener, Logger* logger);
  virtual ~MntServer();
  void getServerInfo(MntServerInfo* info);
  virtual void onConfigReload(ServerConfig* serverConfig);
  void generateExternalShutdownSignal();
  bool isRunningAsService() const;
  virtual void afterFirstClientConnect();
  virtual void afterLastClientDisconnect();

protected:
protected:
  void restartHttpServer();
  void restartControlServer();
  void restartMainRfbServer();

  void stopHttpServer();
  void stopControlServer();
  void stopMainRfbServer();

  // Calls a callback function to change update log properties.
  void changeLogProps();

protected:
  LogWriter m_log;
  ZombieKiller m_zombieKiller;
  ServerConfig* m_srvConfig;
  Configurator m_config;
  LocalMutex m_mutex;
  const bool m_runAsService;
  WinServiceDesktopFactory m_serviceDesktopFactory;
  ApplicationDesktopFactory m_applicationDesktopFactory;
  RfbClientManager* m_rfbClientManager;
  ControlServer* m_controlServer;
  RfbServer* m_rfbServer;
  LogInitListener* m_logInitListener;

  UINT m_contextSwitchResolution;
};
