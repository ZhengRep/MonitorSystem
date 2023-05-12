#pragma once
#include "utils/Command.h"
#include "ControlProxy.h"

class SharePrimaryCommand : public Command
{
public:
  SharePrimaryCommand(ControlProxy* serverControl);
  virtual ~SharePrimaryCommand();

  virtual void execute() throw(IOException, RemoteException);
private:
  ControlProxy* m_proxy;
};

