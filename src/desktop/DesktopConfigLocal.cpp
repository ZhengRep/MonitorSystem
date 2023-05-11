#include "DesktopConfigLocal.h"
#include "server_config_lib/Configurator.h"
#include "desktop/WindowsInputBlocker.h"

DesktopConfigLocal::DesktopConfigLocal(LogWriter* log)
{
  m_inputBlocker = new WindowsInputBlocker(log);
}

DesktopConfigLocal::~DesktopConfigLocal()
{
  delete m_inputBlocker;
}

void DesktopConfigLocal::updateByNewSettings()
{
  ServerConfig* srvConf = Configurator::getInstance()->getServerConfig();
  bool hardBlocking = srvConf->isBlockingLocalInput();
  m_inputBlocker->setKeyboardBlocking(hardBlocking);
  m_inputBlocker->setMouseBlocking(hardBlocking);

  bool softBlocking = srvConf->isLocalInputPriorityEnabled();
  unsigned int interval = srvConf->getLocalInputPriorityTimeout() * 1000;
  m_inputBlocker->setSoftKeyboardBlocking(softBlocking, interval);
  m_inputBlocker->setSoftMouseBlocking(softBlocking, interval);
}

bool DesktopConfigLocal::isRemoteInputAllowed()
{
  return m_inputBlocker->isRemoteInputAllowed();
}

DateTime DesktopConfigLocal::getLastInputTime() const
{
  return m_inputBlocker->getLastInputTime();
}

void DesktopConfigLocal::correctLastTime(DateTime newTime)
{
  m_inputBlocker->correctLastTime(newTime);
}
