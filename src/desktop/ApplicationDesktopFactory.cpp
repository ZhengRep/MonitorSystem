#include "ApplicationDesktopFactory.h"
#include "DesktopWinImpl.h"

ApplicationDesktopFactory::ApplicationDesktopFactory()
{
}

ApplicationDesktopFactory::~ApplicationDesktopFactory()
{
}

Desktop* ApplicationDesktopFactory::createDesktop(ClipboardListener* extClipListener, UpdateSendingListener* extUpdSendingListener, AbnormDeskTermListener* extDeskTermListener, LogWriter* log)
{
  return new DesktopWinImpl(extClipListener, extUpdSendingListener, extDeskTermListener, log);
}
