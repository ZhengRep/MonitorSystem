#pragma once
#include "DesktopServerProto.h"
#include "DesktopSrvDispatcher.h"
#include "utils/DateTime.h"

class DesktopConfigClient: public DesktopServerProto
{
public:
  DesktopConfigClient(BlockingGate* forwGate);
  virtual ~DesktopConfigClient();

  virtual void updateByNewSettings(BlockingGate* gate);
  virtual bool isRemoteInputAllowed();

private:
  DateTime m_lastInputTime;
};

