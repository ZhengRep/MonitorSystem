#pragma once
#include "utils/Command.h"
#include "ControlProxy.h"

// Command that requests server to reload it configuration from the registry
class ReloadConfigCommand : public Command
{
public:
  ReloadConfigCommand(ControlProxy* proxy);
  virtual ~ReloadConfigCommand();

private:
  virtual void execute();

  ControlProxy* m_proxy;
};

