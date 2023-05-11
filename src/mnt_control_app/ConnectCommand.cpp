#include "ConnectCommand.h"

ConnectCommand::ConnectCommand(ControlProxy* serverControl,
  const TCHAR* hostName)
  : m_proxy(serverControl),
  m_hostName(hostName)
{
}

ConnectCommand::~ConnectCommand()
{
}

void ConnectCommand::execute()
{
  m_proxy->makeOutgoingConnection(m_hostName.getString(), false);
}
