#pragma once
#include "DesktopServerProto.h"
#include "desktop/UpdateHandlerImpl.h"
#include "DesktopSrvDispatcher.h"
#include "log_writer/LogWriter.h"
#include "desktop/Win32ScreenDriverFactory.h"

class UpdateHandlerServer : public DesktopServerProto, public ClientListener,
  public UpdateListener
{
public:
  UpdateHandlerServer(BlockingGate* forwGate,
    DesktopSrvDispatcher* dispatcher,
    AnEventListener* extTerminationListener,
    LogWriter* log);
  virtual ~UpdateHandlerServer();

  // Internal dispatcher
  virtual void onRequest(UINT8 reqCode, BlockingGate* backGate);

protected:
  virtual void onUpdate();

  // At first time server must get init information.
  void serverInit(BlockingGate* backGate);

  void extractReply(BlockingGate* backGate);
  void screenPropReply(BlockingGate* backGate);
  void receiveFullReqReg(BlockingGate* backGate);
  void receiveExcludingReg(BlockingGate* backGate);

  Win32ScreenDriverFactory m_scrDriverFactory;

  PixelFormat m_oldPf;

  UpdateHandlerImpl* m_updateHandler;
  AnEventListener* m_extTerminationListener;

  LogWriter* m_log;
};
