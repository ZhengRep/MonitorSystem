#pragma once
#include "utils/Command.h"
#include "ControlProxy.h"

class ShareAppCommand : public Command
{
public:
  ShareAppCommand(ControlProxy* serverControl, unsigned int procId);
  virtual ~ShareAppCommand();

  virtual void execute() throw(IOException, RemoteException);
private:
  ControlProxy* m_proxy;
  unsigned int m_procId;
};

