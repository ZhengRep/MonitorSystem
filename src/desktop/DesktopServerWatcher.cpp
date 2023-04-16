#include "DesktopServerWatcher.h"
#include "winSystem/Environment.h"
#include "Utils/Exception.h"
#include "server_config_lib/Configurator.h"
#include "winSystem/CurrentConsoleProcess.h"
#include "winSystem/AnonymousPipeFactory.h"
#include "winSystem/EmulatedAnonymousPipeFactory.h"
#include "winSystem/WTS.h"
#include "winSystem/WinStaLibrary.h"
#include "winSystem/ShareMemory.h"
#include "winSystem/WinHandles.h"
#include <time.h>

DesktopServerWatcher::DesktopServerWatcher(ReconnectionListener* recListener, LogWriter* log)
{
}

DesktopServerWatcher::~DesktopServerWatcher()
{
}

void DesktopServerWatcher::execute()
{
}

void DesktopServerWatcher::onTerminate()
{
}

void DesktopServerWatcher::start()
{
}

void DesktopServerWatcher::doXPTrick()
{
}
