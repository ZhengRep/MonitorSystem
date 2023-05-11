#pragma once
#include "Command.h"
#include <list>

class MacroCommand : public Command
{
public:
  MacroCommand();

  virtual ~MacroCommand();

  virtual void execute();

  void addCommand(Command* command);

private:

  std::list<Command*> m_commandList;
};

