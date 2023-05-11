#include "UpdateRemoteConfigCommand.h"
#include "server_config_lib/Configurator.h"

UpdateRemoteConfigCommand::UpdateRemoteConfigCommand(ControlProxy* serverControl)
  : m_serverControl(serverControl)
{
}

UpdateRemoteConfigCommand::~UpdateRemoteConfigCommand()
{
}

void UpdateRemoteConfigCommand::execute()
{
  m_serverControl->setServerConfig(Configurator::getInstance()->getServerConfig());
}
