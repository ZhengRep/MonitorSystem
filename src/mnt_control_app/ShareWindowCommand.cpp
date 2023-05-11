#include "ShareWindowCommand.h"

ShareWindowCommand::ShareWindowCommand(ControlProxy* serverControl,
  const StringStorage* shareWindowName)
  : m_proxy(serverControl),
  m_shareWindowName(*shareWindowName)
{
}

ShareWindowCommand::~ShareWindowCommand()
{
}

void ShareWindowCommand::execute()
{
  m_proxy->shareWindow(&m_shareWindowName);
}
