#pragma once
#include "Thread/GuiThread.h"
#include "BlockingGate.h"
#include "ClientListener.h"
#include "utils/AnEventListener.h"
#include "win_system/WindowsEvent.h"
#include "log_writer/LogWriter.h"
#include <map>

class DesktopSrvDispatcher:public GuiThread
{
public:
  DesktopSrvDispatcher(BlockingGate* gate, AnEventListener* extErrorListener, LogWriter* log);
  virtual ~DesktopSrvDispatcher();

  void registerNewHandle(UINT8 code, ClientListener* listener);

protected:
  virtual void execute();
  virtual void onTerminate();
  void notifyOnError();

  BlockingGate* m_gate;
  std::map<UINT8, ClientListener*> m_handlers;
  AnEventListener* m_extErrorListener;
  LogWriter* m_log;
};

