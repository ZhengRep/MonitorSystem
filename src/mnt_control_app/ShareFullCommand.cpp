#include "ShareFullCommand.h"

ShareFullCommand::ShareFullCommand(ControlProxy* serverControl)
  : m_proxy(serverControl)
{
}

ShareFullCommand::~ShareFullCommand()
{
}

void ShareFullCommand::execute()
{
  m_proxy->shareFull();
}
