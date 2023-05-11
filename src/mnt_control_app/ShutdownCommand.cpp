#include "ShutdownCommand.h"

ShutdownCommand::ShutdownCommand(ControlProxy* serverControl)
  : m_proxy(serverControl)
{
}

ShutdownCommand::~ShutdownCommand()
{
}

void ShutdownCommand::execute()
{
  m_proxy->shutdownMnt();
}
