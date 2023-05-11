#include "WinServiceDesktopFactory.h"
#include "DesktopClientImpl.h"

WinServiceDesktopFactory::WinServiceDesktopFactory()
{
}

WinServiceDesktopFactory::~WinServiceDesktopFactory()
{
}

Desktop* WinServiceDesktopFactory::createDesktop(ClipboardListener* extClipListener, 
  UpdateSendingListener* extUpdSendingListener, AbnormDeskTermListener* extDeskTermListener, LogWriter* log)
{
  return new DesktopClientImpl(extClipListener, extUpdSendingListener, extDeskTermListener, log);
}
