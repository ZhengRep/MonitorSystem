#pragma once
#include "DesktopSrvDispatcher.h"

class GateKickHandler : public ClientListener
{
public:
  GateKickHandler(DesktopSrvDispatcher* dispatcher);
  virtual ~GateKickHandler();
protected:
  virtual void onRequest(UINT8 reqCode, BlockingGate* backGate);
};

