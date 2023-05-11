#include "ReloadConfigCommand.h"

ReloadConfigCommand::ReloadConfigCommand(ControlProxy* proxy)
  : m_proxy(proxy)
{
}

ReloadConfigCommand::~ReloadConfigCommand()
{
}

void ReloadConfigCommand::execute()
{
  m_proxy->reloadServerConfig();
}
