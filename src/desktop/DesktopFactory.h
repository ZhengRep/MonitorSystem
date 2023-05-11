#pragma once
#include "ClipboardListener.h"
#include "UpdateSendingListener.h"
#include "AbnormDeskTermListener.h"
#include "log_writer/LogWriter.h"
#include "Desktop.h"

class DesktopFactory
{
public:
  virtual Desktop* createDesktop(ClipboardListener* extClipListener, UpdateSendingListener* extUpdateSendingListener,
    AbnormDeskTermListener* extDeskTermListener, LogWriter* log) = 0;
};