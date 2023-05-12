#pragma once
#include "utils/CommonHeader.h"
#include "DesktopServerProto.h"
#include "ClientListener.h"
#include "desktop/WindowsUserInput.h"
#include "win_system/WindowsEvent.h"
#include "DesktopSrvDispatcher.h"
#include "log_writer/LogWriter.h"

class UserInputServer : public DesktopServerProto, public ClientListener,
  public ClipboardListener
{
public:
  UserInputServer(BlockingGate* forwGate,
    DesktopSrvDispatcher* dispatcher,
    AnEventListener* extTerminationListener,
    LogWriter* log);
  virtual ~UserInputServer();

  // Internal dispatcher
  virtual void onRequest(UINT8 reqCode, BlockingGate* backGate);

  virtual void onClipboardUpdate(const StringStorage* newClipboard);

protected:
  virtual void applyNewPointerPos(BlockingGate* backGate);
  virtual void applyNewClipboard(BlockingGate* backGate);
  virtual void applyKeyEvent(BlockingGate* backGate);
  virtual void ansDesktopCoords(BlockingGate* backGate);
  virtual void ansWindowCoords(BlockingGate* backGate);
  virtual void ansUserInfo(BlockingGate* backGate);
  virtual void ansWindowHandle(BlockingGate* backGate);
  virtual void ansDisplayNumberCoords(BlockingGate* backGate);
  virtual void ansApplicationRegion(BlockingGate* backGate);
  virtual void ansApplicationInFocus(BlockingGate* backGate);
  virtual void ansNormalizeRect(BlockingGate* backGate);

  // At first time server must get init information.
  void serverInit(BlockingGate* backGate);

  WindowsUserInput* m_userInput;
  AnEventListener* m_extTerminationListener;

  LogWriter* m_log;
};


