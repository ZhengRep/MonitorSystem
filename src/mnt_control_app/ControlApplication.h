#pragma once
#include "Transport.h"
#include "TransportFactory.h"
#include "Utils/CommonHeader.h"
#include "Utils/ResourceLoader.h"
#include "Utils/Command.h"
#include "server_config_lib/Configurator.h"
#include "log_writer/LogWriter.h"
#include "thread/Thread.h"
#include "thread/LocalMutex.h"
#include "ControlGate.h"
#include "ControlAuthException.h"
#include "ControlProxy.h"
#include "Notificator.h"
#include "winSystem/WindowsApplication.h"

class ControlTrayIcon;

class ControlApplication : public WindowsApplication,
  public Notificator,
  private Thread 
{
public:
  ControlApplication(HINSTANCE hinst, 
    const TCHAR* windowClassName, const TCHAR* commandLine);
  virtual ~ControlApplication();
  virtual int run();

  virtual void notifyServerSideException(const TCHAR* reason);
  virtual void notifyConnectionLost();

protected:
  void connect(bool controlService, bool slave) throw(Exception);
  void execute();
  int runControlInterface(bool showIcon);
  int runControlCommand(Command* command);
  int runConfigurator(bool configService, bool isRunAsRequested);
  int checkServicePasswords(bool isRunAsRequested);

private:
  static void getCryptedPassword(UINT8 cryptedPass[8], const TCHAR* plainTextPass);
  void checkServicePasswords();
  void reloadConfig();

private:
  LogWriter m_log;

  Configurator m_configurator;

protected:

  // Low-level transport to control server (proxy).
  Transport* m_transport;

  ControlGate* m_gate;
  // High-level transport to control server (proxy).
  ControlProxy* m_serverControl;

  ControlTrayIcon* m_trayIcon;

  StringStorage m_commandLine;

  bool m_slaveModeEnabled;
  friend class ControlTrayIcon;
};

