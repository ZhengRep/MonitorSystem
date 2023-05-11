#include "Win32ScreenDriverFactory.h"
#include "Win32ScreenDriver.h"

Win32ScreenDriverFactory::Win32ScreenDriverFactory(ServerConfig* srvConf)
  : m_srvConf(srvConf)
{
}

Win32ScreenDriverFactory::~Win32ScreenDriverFactory()
{
}

ScreenDriver* Win32ScreenDriverFactory::createScreenDriver(UpdateKeeper* updateKeeper, UpdateListener* updateListener, FrameBuffer* fb, LocalMutex* fbLocalMutex, LogWriter* log)
{
  log->info(_T("Using the standart screen driver"));
  return createStandardScreenDriver(updateKeeper,
    updateListener,
    fb,
    fbLocalMutex, log);
}

ScreenDriver* Win32ScreenDriverFactory::createStandardScreenDriver(UpdateKeeper* updateKeeper, UpdateListener* updateListener, FrameBuffer* fb, LocalMutex* fbLocalMutex, LogWriter* log)
{
  return new Win32ScreenDriver(updateKeeper, updateListener, fb, fbLocalMutex, log);
}

