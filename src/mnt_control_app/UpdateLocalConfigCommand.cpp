#include "UpdateLocalConfigCommand.h"
#include "server_config_lib/Configurator.h"

UpdateLocalConfigCommand::UpdateLocalConfigCommand(ControlProxy* proxy)
  : m_proxy(proxy)
{
}

UpdateLocalConfigCommand::~UpdateLocalConfigCommand()
{
}

void UpdateLocalConfigCommand::execute()
{
  m_proxy->getServerConfig(Configurator::getInstance()->getServerConfig());
}
