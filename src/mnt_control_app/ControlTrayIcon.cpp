#include "ControlTrayIcon.h"
#include "ControlApplication.h"
#include "UpdateRemoteConfigCommand.h"
#include "DisconnectCommand.h"
#include "ShutdownCommand.h"
#include "ControlCommand.h"
#include "UpdateLocalConfigCommand.h"
#include "Utils/ResourceLoader.h"
#include "Utils/StringTable.h"
#include "Utils/MacroCommand.h"
#include "mnt_server/resource.h"

#include <crtdbg.h>

UINT ControlTrayIcon::WM_USER_TASKBAR;

ControlTrayIcon::ControlTrayIcon(ControlProxy* serverControl,
  Notificator* notificator,
  ControlApplication* appControl,
  bool showAfterCreation)
  : NotifyIcon(showAfterCreation),
  m_serverControl(serverControl), m_notificator(notificator),
  m_appControl(appControl),
  m_inWindowProc(false),
  m_termination(false)
{
  ResourceLoader* resLoader = ResourceLoader::getInstance();

  m_iconWorking = new Icon(resLoader->loadIcon(MAKEINTRESOURCE(IDI_CONNECTED)));
  m_iconIdle = new Icon(resLoader->loadIcon(MAKEINTRESOURCE(IDI_IDLE)));
  m_iconDisabled = new Icon(resLoader->loadIcon(MAKEINTRESOURCE(IDI_DISABLED)));

  setWindowProcHolder(this);

  // Prepare commands for configration dialog.
  m_updateRemoteConfigCommand = new UpdateRemoteConfigCommand(m_serverControl);
  m_updateLocalConfigCommand = new UpdateLocalConfigCommand(m_serverControl);
  m_applyChangesMacroCommand = new MacroCommand();
  m_applyChangesMacroCommand->addCommand(m_updateRemoteConfigCommand);
  m_applyChangesControlCommand = new ControlCommand(m_applyChangesMacroCommand, m_notificator);

  // Create config dialog.
  m_configDialog = new ConfigDialog();
  m_configDialog->setConfigReloadCommand(m_applyChangesControlCommand);

  // Default icon state.
  setNotConnectedState();

  // Update status.
  syncStatusWithServer();

  WM_USER_TASKBAR = RegisterWindowMessage(_T("TaskbarCreated"));
}

ControlTrayIcon::~ControlTrayIcon()
{
  delete m_configDialog;

  delete m_applyChangesControlCommand;
  delete m_applyChangesMacroCommand;
  delete m_updateLocalConfigCommand;
  delete m_updateRemoteConfigCommand;

  delete m_iconDisabled;
  delete m_iconIdle;
  delete m_iconWorking;
}

LRESULT ControlTrayIcon::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
  bool* useDefWindowProc)
{
  if (m_inWindowProc) {
    // This call is recursive, do not do any real work.
    *useDefWindowProc = true;
    return 0;
  }
  if (m_termination) {
    m_endEvent.notify();
    *useDefWindowProc = true;
    return 0;
  }

  // Make sure to reset it back to false before leaving this function for any
  // reason (check all return statements, exceptions should not happen here).
  m_inWindowProc = true;

  switch (uMsg) {
  case WM_USER + 1:
    switch (lParam) {
    case WM_RBUTTONUP:
      onRightButtonUp();
      break;
    case WM_LBUTTONDOWN:
      onLeftButtonDown();
      break;
    } // switch (lParam)
    break;
  default:
    if (uMsg == WM_USER_TASKBAR) {
      hide();
    }
    *useDefWindowProc = true;
  }

  m_inWindowProc = false;
  return 0;
}

void ControlTrayIcon::onRightButtonUp()
{
  HMENU hRoot = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_TRAYMENU));
  HMENU hMenu = GetSubMenu(hRoot, 0);

  SetMenuDefaultItem(hMenu, ID_CONFIGURATION, FALSE);

  if (m_appControl->m_slaveModeEnabled) {
    RemoveMenu(hMenu, ID_CLOSE_CONTROL_INTERFACE, MF_BYCOMMAND);
  }

  POINT pos;

  if (!GetCursorPos(&pos)) {
    pos.x = pos.y = 0;
  }

  SetForegroundWindow(getWindow());

  int action = TrackPopupMenu(hMenu,
    TPM_NONOTIFY | TPM_RETURNCMD | TPM_RIGHTBUTTON,
    pos.x, pos.y, 0, getWindow(), NULL);

  switch (action) {
  case ID_KILLCLIENTS:
    onDisconnectAllClientsMenuItemClick();
    break;
  case ID_SHUTDOWN_SERVICE:
    onShutdownServerMenuItemClick();
    break;
  case ID_CONFIGURATION:
    onConfigurationMenuItemClick();
    break;
  case ID_OUTGOING_CONN:
    onOutgoingConnectionMenuItemClick();
    break;
  case ID_CLOSE_CONTROL_INTERFACE:
    onCloseControlInterfaceMenuItemClick();
    break;
  }
}

void ControlTrayIcon::onLeftButtonDown()
{
  onConfigurationMenuItemClick();
}

void ControlTrayIcon::onConfigurationMenuItemClick()
{
  ControlApplication::removeModelessDialog(m_configDialog->getControl()->getWindow());

  bool isConnectedToService = false;

  try {
    isConnectedToService = m_serverControl->getServerInfo().m_serviceFlag;
  } catch (...) {
    return;
  }

  Configurator::getInstance()->setServiceFlag(isConnectedToService);

  // Copy running tightvnc config to our global server config.
  if (!m_configDialog->isCreated()) {
    UpdateLocalConfigCommand updateLocalConfigCommand(m_serverControl);

    ControlCommand safeCommand(&updateLocalConfigCommand, m_appControl);

    safeCommand.execute();

    if (!safeCommand.executionResultOk()) {
      return;
    }
  }

  // Show dialog.
  m_configDialog->setServiceFlag(isConnectedToService);
  m_configDialog->show();

  ControlApplication::addModelessDialog(m_configDialog->getControl()->getWindow());
}

void ControlTrayIcon::onDisconnectAllClientsMenuItemClick()
{
  DisconnectAllCommand unsafeCommand(m_serverControl);

  ControlCommand safeCommand(&unsafeCommand, m_notificator);

  safeCommand.execute();
}

void ControlTrayIcon::onShutdownServerMenuItemClick()
{
  // Promt user if any client is connected to rfb server.

  // FIXME: Bad way to determinate connected clients.
  bool someoneConnected = (getIcon() == m_iconWorking);

  if (someoneConnected) {
    MntServerInfo serverInfo = { 0 };

    {
      AutoLock l(&m_serverInfoMutex);

      serverInfo = m_lastKnownServerInfo;
    }

    StringStorage userMessage;

    UINT stringId = serverInfo.m_serviceFlag ? IDS_MNTSERVER_SERVICE : IDS_MNTSERVER_APP;

    userMessage.format(
      StringTable::getString(IDS_SHUTDOWN_NOTIFICATION_FORMAT),
      StringTable::getString(stringId));

    if (MessageBox(
      getWindow(),
      userMessage.getString(),
      StringTable::getString(IDS_MBC_MNTCONTROL),
      MB_YESNO | MB_ICONQUESTION) == IDNO) {
      return;
    }
  }

  // Shutdown TightVNC server.

  ShutdownCommand unsafeCommand(m_serverControl);

  ControlCommand safeCommand(&unsafeCommand, m_notificator);

  safeCommand.execute();
}

void ControlTrayIcon::onOutgoingConnectionMenuItemClick()
{
  
}

void ControlTrayIcon::onAboutMenuItemClick()
{
 /* m_aboutDialog.show();

  ControlApplication::addModelessDialog(m_aboutDialog.getControl()->getWindow());*/
}

void ControlTrayIcon::onCloseControlInterfaceMenuItemClick()
{
  m_appControl->shutdown();
}

void ControlTrayIcon::syncStatusWithServer()
{
  try {
    MntServerInfo info = m_serverControl->getServerInfo();
    std::list<RfbClientInfo*> clients;
    m_serverControl->getClientsList(&clients);

    // Change icon status.
    if (clients.size() > 0) {
      setIcon(m_iconWorking);
    } else if (info.m_acceptFlag) {
      setIcon(m_iconIdle);
    } else {
      setIcon(m_iconDisabled);
    }

    setText(info.m_statusText.getString());

    // Cleanup.
    for (std::list<RfbClientInfo*>::iterator it = clients.begin(); it != clients.end(); it++) {
      delete* it;
    }

    {
      AutoLock l(&m_serverInfoMutex);

      m_lastKnownServerInfo = info;
    }
  } catch (IOException&) {
    setNotConnectedState();
  } catch (Exception&) {
    _ASSERT(FALSE);
  } // try / catch.
}

void ControlTrayIcon::setNotConnectedState()
{
  setIcon(m_iconDisabled);
  setText(StringTable::getString(IDS_CONTROL_CLIENT_NOT_CONNECTED));
}

void ControlTrayIcon::terminate()
{
  m_termination = true;
  // Forcing window message
  PostMessage(getWindow(), WM_USER + 1, 0, 0);
}

void ControlTrayIcon::waitForTermination()
{
  m_endEvent.waitForEvent();
}
