#pragma once
#include "Utils/Command.h"
#include "ControlProxy.h"

class DisconnectAllCommand : public Command
{
public:
  DisconnectAllCommand(ControlProxy* serverControl);

  virtual ~DisconnectAllCommand();
  virtual void execute() throw(IOException, RemoteException);

protected:

  ControlProxy* m_proxy;
};


