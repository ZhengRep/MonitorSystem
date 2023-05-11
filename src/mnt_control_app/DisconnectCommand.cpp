#include "DisconnectCommand.h"

DisconnectAllCommand::DisconnectAllCommand(ControlProxy* serverControl)
  : m_proxy(serverControl)
{
}

DisconnectAllCommand::~DisconnectAllCommand()
{
}

void DisconnectAllCommand::execute()
{
  m_proxy->disconnectAllClients();
}
