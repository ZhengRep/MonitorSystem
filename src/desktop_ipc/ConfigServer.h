#pragma once
#include "DesktopSrvDispatcher.h"
#include "DesktopServerProto.h"
#include "desktop/DesktopConfigLocal.h"

class ConfigServer : public DesktopServerProto, public ClientListener
{
public:
  ConfigServer(DesktopSrvDispatcher* dispatcher, LogWriter* log);
  virtual ~ConfigServer();

protected:
  // Internal dispatcher
  virtual void onRequest(UINT8 reqCode, BlockingGate* backGate);

  void reloadSettings(BlockingGate* backGate);
  void answerOnSoftInputEnablingReq(BlockingGate* backGate);

  DesktopConfigLocal m_deskConf;
};

