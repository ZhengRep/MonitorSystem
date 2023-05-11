#pragma once
#include "gui/NotifyIcon.h"
#include "gui/WindowProcHolder.h"
#include "gui/drawing/Icon.h"
#include "thread/Thread.h"
#include "thread/LocalMutex.h"
#include "Utils/Command.h"
#include "Utils/MacroCommand.h"
#include "wsconfig_lib/ConfigDialog.h"
#include "ControlProxy.h"
#include "Notificator.h"
#include "ControlApplication.h"

class ControlTrayIcon : public NotifyIcon, 
  public WindowProcHolder 
{
public:
   ControlTrayIcon(ControlProxy* serverControl, Notificator* notificator,
    ControlApplication* appControl, bool showAfterCreation);
  virtual ~ControlTrayIcon();


  void syncStatusWithServer();

  void terminate();

  void waitForTermination();

protected:

  void setNotConnectedState();

  virtual LRESULT windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* useDefWindowProc);

  void onRightButtonUp();
  void onLeftButtonDown();

  void onConfigurationMenuItemClick();
  void onDisconnectAllClientsMenuItemClick();
  void onShutdownServerMenuItemClick();
  void onOutgoingConnectionMenuItemClick();
  void onAttachToDispatcher();
  void onAboutMenuItemClick();
  void onCloseControlInterfaceMenuItemClick();

protected:
  static UINT WM_USER_TASKBAR;

protected:

  Notificator* m_notificator;

  // Pointer to control application.
  ControlApplication* m_appControl;

  // States of tray icon.
  Icon* m_iconWorking;
  Icon* m_iconIdle;
  Icon* m_iconDisabled;

  // Interface to execute some commands on remote TightVNC server.
  ControlProxy* m_serverControl;

  // Configuration dialog.
  ConfigDialog* m_configDialog;

  MntServerInfo m_lastKnownServerInfo;
  LocalMutex m_serverInfoMutex;

  // Commands for configuration dialog.
  Command* m_updateRemoteConfigCommand;
  Command* m_updateLocalConfigCommand;
  MacroCommand* m_applyChangesMacroCommand;
  ControlCommand* m_applyChangesControlCommand;

  bool m_inWindowProc;

  WindowsEvent m_endEvent;
  bool m_termination;
};

