#include "ShareAppCommand.h"

ShareAppCommand::ShareAppCommand(ControlProxy* serverControl, unsigned int procId)
  : m_proxy(serverControl),
  m_procId(procId)
{
}

ShareAppCommand::~ShareAppCommand()
{
}

void ShareAppCommand::execute()
{
  m_proxy->shareApp(m_procId);
}
