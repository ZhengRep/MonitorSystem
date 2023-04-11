#include "WinServiceDesktopFactory.h"

WinServiceDesktopFactory::WinServiceDesktopFactory()
{
}

WinServiceDesktopFactory::~WinServiceDesktopFactory()
{
}

Desktop* WinServiceDesktopFactory::createDesktop(ClipboardListener* extClipListener, UpdateSendingListener* extUpdSendingListener, AbnormDeskTermListener* extDeskTermListener, LogWriter* log)
{
    return new DesktopClient
}
