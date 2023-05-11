#pragma once
#include "Utils/Command.h"
#include "ControlProxy.h"

//Command that copies current configuration of server
// to local application
class UpdateLocalConfigCommand : public Command
{
public:
  UpdateLocalConfigCommand(ControlProxy* proxy);
  virtual ~UpdateLocalConfigCommand();
  virtual void execute() throw(IOException, RemoteException);
private:
  ControlProxy* m_proxy;
};