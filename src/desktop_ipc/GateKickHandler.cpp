#include "GateKickHandler.h"

GateKickHandler::GateKickHandler(DesktopSrvDispatcher* dispatcher)
{
  dispatcher->registerNewHandle(255, this);
}

GateKickHandler::~GateKickHandler()
{
}

void GateKickHandler::onRequest(UINT8 reqCode, BlockingGate* backGate)
{
  // Do nothing
}
