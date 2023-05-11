#include "ShareDisplayCommand.h"

ShareDisplayCommand::ShareDisplayCommand(ControlProxy* serverControl,
  unsigned char displayNumber)
  : m_proxy(serverControl),
  m_displayNumber(displayNumber)
{
}

ShareDisplayCommand::~ShareDisplayCommand()
{
}

void ShareDisplayCommand::execute()
{
  m_proxy->shareDisplay(m_displayNumber);
}
