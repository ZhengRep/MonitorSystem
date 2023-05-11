#include "ShareRectCommand.h"

ShareRectCommand::ShareRectCommand(ControlProxy* serverControl,
  const Rect* shareRect)
  : m_proxy(serverControl),
  m_shareRect(shareRect)
{
}

ShareRectCommand::~ShareRectCommand()
{
}

void ShareRectCommand::execute()
{
  m_proxy->shareRect(&m_shareRect);
}
