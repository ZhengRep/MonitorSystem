#pragma once
#include "utils/Exception.h"
#include "utils/Singleton.h"
#include "log_writer/LogWriter.h"
#include "win_system/Impersonator.h"
#include "desktop/Desktop.h"

class FileTransferSecurity: private Impersonator
{
public:
  FileTransferSecurity(Desktop* desktop, LogWriter* log);
  virtual ~FileTransferSecurity();
  void beginMessageProcessing();
  void throwIfAccessDenied() throw(Exception);
  void endMessageProcessing();

protected:
  bool m_hasAccess;
  Desktop* m_desktop;
  LogWriter* m_log;
};


