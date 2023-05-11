#pragma once
#include "ScreenDriverFactory.h"
#include "server_config_lib/ServerConfig.h"

class Win32ScreenDriverFactory : public ScreenDriverFactory
{
public:
  Win32ScreenDriverFactory(ServerConfig* srvConf);
  virtual ~Win32ScreenDriverFactory();

  virtual ScreenDriver* createScreenDriver(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
    FrameBuffer* fb, LocalMutex* fbLocalMutex, LogWriter* log);
private:
  ScreenDriver* createStandardScreenDriver(UpdateKeeper* updateKeeper, UpdateListener* updateListener,
    FrameBuffer* fb, LocalMutex* fbLocalMutex, LogWriter* log);

  ServerConfig* m_srvConf;
};

