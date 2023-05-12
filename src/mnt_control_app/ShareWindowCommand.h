#pragma once
#include "utils/Command.h"
#include "ControlProxy.h"

class ShareWindowCommand : public Command
{
public:
  ShareWindowCommand(ControlProxy* serverControl,
    const StringStorage* shareWindowName);
  virtual ~ShareWindowCommand();

  virtual void execute() throw(IOException, RemoteException);
private:
  ControlProxy* m_proxy;
  StringStorage m_shareWindowName;
};

