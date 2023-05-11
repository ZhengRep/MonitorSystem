#pragma once
#include "Utils/Command.h"
#include "Notificator.h"

class ControlCommand : public Command
{
public:
  ControlCommand(Command* command, Notificator* notificator = 0);
  virtual ~ControlCommand();
  virtual void execute();
  bool executionResultOk() const;

protected:

  Command* m_command;

  Notificator* m_notificator;

  bool m_successfull;
};

