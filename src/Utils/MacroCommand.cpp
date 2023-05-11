#include "MacroCommand.h"

MacroCommand::MacroCommand()
{
}

MacroCommand::~MacroCommand()
{
}

void MacroCommand::execute()
{
  for (std::list<Command*>::iterator it = m_commandList.begin();
    it != m_commandList.end();
    it++) {
    (*it)->execute();
  }
}

void MacroCommand::addCommand(Command* command)
{
  m_commandList.push_back(command);
}
