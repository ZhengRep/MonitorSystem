#pragma once
#include "Utils/Command.h"
#include "ControlProxy.h"

class ShareFullCommand : public Command
{
public:
  ShareFullCommand(ControlProxy* serverControl);
  virtual ~ShareFullCommand();

  virtual void execute() throw(IOException, RemoteException);
private:
  ControlProxy* m_proxy;
};


