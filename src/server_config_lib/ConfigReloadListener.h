#pragma once
#include "ServerConfig.h"

class ConfigReloadListener
{
public:
  ConfigReloadListener(){};
  virtual ~ConfigReloadListener(){};

  virtual void onConfigReload(ServerConfig* serverConfig) = 0;
};

