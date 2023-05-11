#include "FileTransferSecurity.h"
#include "server_config_lib/Configurator.h"

FileTransferSecurity::FileTransferSecurity(Desktop* desktop, LogWriter* log)
  : Impersonator(log),
  m_hasAccess(false),
  m_desktop(desktop),
  m_log(log)
{
  m_desktop = desktop;
}

FileTransferSecurity::~FileTransferSecurity()
{
}

void FileTransferSecurity::beginMessageProcessing()
{
  bool runAsService = Configurator::getInstance()->getServiceFlag();
  if (!runAsService) {
    m_hasAccess = true;
  } else {
    try {
      StringStorage userName, desktopName;

      if (m_desktop != NULL) {
        m_desktop->getCurrentUserInfo(&desktopName, &userName);
      }

      desktopName.toLowerCase();

      if (!desktopName.isEqualTo(_T("default"))) {
        throw Exception(_T("Desktop is not default desktop."));
      }

      impersonateAsLoggedUser();

      m_hasAccess = true;
    } catch (Exception& e) {
      m_log->error(_T("Access denied to the file transfer: %s"), e.getMessage());
      m_hasAccess = false;
    } // try / catch.
  } // if running as service.
}

void FileTransferSecurity::throwIfAccessDenied() throw(Exception)
{
  if (!m_hasAccess) {
    throw Exception(_T("Access denied."));
  } else if (!Configurator::getInstance()->getServerConfig()->isFileTransfersEnabled()) {
    throw Exception(_T("File transfers are disabled on server side."));
  }
}

void FileTransferSecurity::endMessageProcessing()
{
  if (Configurator::getInstance()->getServiceFlag() && m_hasAccess) {
    try {
      revertToSelf();
    } catch (...) {
    } // try / catch.
  } // if run as service.
}
