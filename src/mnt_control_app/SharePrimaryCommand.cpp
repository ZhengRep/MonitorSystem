#include "SharePrimaryCommand.h"

SharePrimaryCommand::SharePrimaryCommand(ControlProxy* serverControl)
  : m_proxy(serverControl)
{
}

SharePrimaryCommand::~SharePrimaryCommand()
{
}

void SharePrimaryCommand::execute()
{
  m_proxy->sharePrimary();
}
