#pragma once
#include "UpdateDetector.h"
#include "winSystem/WindowsEvent.h"
#include "HookInstaller.h"
#include "gui/MessageWindow.h"
#include "HookUpdateTimer.h"
#include "winSystem/Process.h"
#include "log_writer/LogWriter.h"

class HooksUpdateDetector : public UpdateDetector
{
public:
  HooksUpdateDetector(UpdateKeeper* updateKeeper,
    UpdateListener* updateListener, LogWriter* log);
  virtual ~HooksUpdateDetector();

protected:
  virtual void execute();
  virtual void onTerminate();

  void start32Loader();
  void terminate32Loader();

  WindowsEvent m_initWaiter;

  HookInstaller* m_hookInstaller;
  MessageWindow* m_targetWin;
  HookUpdateTimer m_updateTimer;
  Process m_hookLoader32;

private:
  void broadcastMessage(UINT message);

  static const TCHAR HOOK_LOADER_NAME[];

  LogWriter* m_log;
};
