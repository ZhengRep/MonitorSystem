#pragma once
#include "Utils/Command.h"
#include "ControlProxy.h"

class ConnectCommand : public Command
{
public:

  ConnectCommand(ControlProxy* serverControl, const TCHAR* hostName);

  virtual ~ConnectCommand();

  virtual void execute() throw(IOException, RemoteException);
private:

  ControlProxy* m_proxy;
  StringStorage m_hostName;
};
