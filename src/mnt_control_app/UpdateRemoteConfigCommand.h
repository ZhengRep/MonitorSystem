#pragma once
#include "utils/Command.h"
#include "ControlProxy.h"

/**
 * Command that sends current configuration to Server.
 * This command will ask server to save recieved configuration.
 */
class UpdateRemoteConfigCommand : public Command
{
public:
  UpdateRemoteConfigCommand(ControlProxy* serverControl);

  virtual ~UpdateRemoteConfigCommand();
  virtual void execute() throw(IOException, RemoteException);

protected:

  ControlProxy* m_serverControl;
};

