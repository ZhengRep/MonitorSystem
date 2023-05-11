#pragma once
#include "MntServer.h"
#include "MntServerListener.h"
#include "log_server/LogServer.h"
#include "log_server/ClientLogger.h"
#include "winSystem/Service.h"
#include "thread/Thread.h"
#include "WinServiceEvents.h"
#include "NewConnectionEvents.h"

class MntService : public Service,
  public MntServerListener,
  private LogInitListener
{
public:
  static const TCHAR SERVICE_COMMAND_LINE_KEY[];
  MntService(WinServiceEvents* winServiceEvents, 
    NewConnectionEvents* newConnectionEvents);
  virtual ~MntService();
  virtual void onMntServerShutdown();
  static void install() throw(SystemException);
  static void remove() throw(SystemException);
  static void reinstall() throw(SystemException);
  static void start(bool waitCompletion = false) throw(SystemException);
  static void stop(bool waitCompletion = false) throw(SystemException);

protected:
  virtual void onStart() throw(SystemException);
  virtual void onStop();
  virtual void main();
  static bool getBinPath(StringStorage* binPath);
  virtual void onLogInit(const TCHAR* logDir, const TCHAR* fileName, unsigned char logLevel);
  virtual void onChangeLogProps(const TCHAR* newLogDir, unsigned char newLevel);

protected:

  WindowsEvent m_shutdownEvent;

  MntServer* m_mntServer;

  LogServer m_logServer;
  ClientLogger m_clientLogger;

  WinServiceEvents* m_winServiceEvents;
  NewConnectionEvents* m_newConnectionEvents;

};

