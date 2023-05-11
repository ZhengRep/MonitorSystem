#pragma once
#include "Utils/Command.h"
#include "ControlProxy.h"

class ShutdownCommand : public Command
{
public:
  ShutdownCommand(ControlProxy* serverControl);

  virtual ~ShutdownCommand();
  virtual void execute() throw(IOException, RemoteException);

protected:

  ControlProxy* m_proxy;
};

