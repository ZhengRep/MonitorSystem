#pragma once
#include "DesktopFactory.h"

class WinServiceDesktopFactory: public DesktopFactory
{
public:
  WinServiceDesktopFactory();
  ~WinServiceDesktopFactory();
  virtual Desktop* createDesktop(ClipboardListener* extClipListener, UpdateSendingListener* extUpdSendingListener,
    AbnormDeskTermListener* extDeskTermListener, LogWriter* log);


};

