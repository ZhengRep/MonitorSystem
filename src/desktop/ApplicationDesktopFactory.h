#pragma once
#include "DesktopFactory.h"

class ApplicationDesktopFactory: public DesktopFactory
{
public:
  ApplicationDesktopFactory();
  ~ApplicationDesktopFactory();

  virtual Desktop* createDesktop(ClipboardListener* extClipListener,
    UpdateSendingListener* extUpdSendingListener, AbnormDeskTermListener* extDeskTermListener,
    LogWriter* log);
};

