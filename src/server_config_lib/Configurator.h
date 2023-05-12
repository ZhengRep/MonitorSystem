#pragma once
#include "config_lib/SettingsManager.h"
#include "PortMappingContainer.h"
#include "IpAccessControl.h"
#include "ServerConfig.h"
#include "ConfigReloadListener.h"
#include "RegistrySecurityAttributes.h"
#include "utils/ListenerContainer.h"

class Configurator: public ListenerContainer<ConfigReloadListener*>
{
public:
  Configurator(bool isConfiguringService);
  virtual ~Configurator();

public:

  bool getServiceFlag() { return m_isConfiguringService; }
  void setServiceFlag(bool asService) { m_isConfiguringService = asService; }

  bool load();
  bool save();
  bool isConfigLoadedPartly() { return m_isConfigLoadedPartly; }

  static Configurator* getInstance();
  static void setInstance(Configurator* conf);
  void notifyReload();
  ServerConfig* getServerConfig() { return &m_serverConfig; }

private:
  //
 // Serialize and deserialize methods
 //

  bool savePortMappingContainer(SettingsManager* sm);
  bool loadPortMappingContainer(SettingsManager* sm, PortMappingContainer* portMapping);

  bool saveIpAccessControlContainer(SettingsManager* sm);
  bool loadIpAccessControlContainer(SettingsManager* sm, IpAccessControl* ipContainer);

  bool saveServerConfig(SettingsManager* sm);
  bool loadServerConfig(SettingsManager* sm, ServerConfig* config);
  void updateLogDirPath();

  bool saveQueryConfig(SettingsManager* sm);
  bool loadQueryConfig(SettingsManager* sm, ServerConfig* config);

  bool saveInputHandlingConfig(SettingsManager* sm);
  bool loadInputHandlingConfig(SettingsManager* sm, ServerConfig* config);

  bool saveVideoRegionConfig(SettingsManager* sm);
  bool loadVideoRegionConfig(SettingsManager* sm, ServerConfig* config);

  bool save(SettingsManager* sm);
  bool load(SettingsManager* sm);

  bool load(bool forService);
  bool save(bool forService);

protected:
  ServerConfig m_serverConfig;

  bool m_isConfiguringService;
  bool m_isConfigLoadedPartly;

  RegistrySecurityAttributes* m_regSA;

  bool m_isFirstLoad;

  static Configurator* s_instance;
  static LocalMutex m_instanceMutex;
};

