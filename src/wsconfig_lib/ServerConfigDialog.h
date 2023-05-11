#pragma once
#include "gui/BaseDialog.h"
#include "gui/TextBox.h"
#include "gui/CheckBox.h"
#include "gui/SpinControl.h"
#include "gui/BalloonTip.h"
#include "server_config_lib/ServerConfig.h"
#include "PasswordControl.h"

class ServerConfigDialog : public BaseDialog
{
public:
  ServerConfigDialog();
  virtual ~ServerConfigDialog();

  void setParentDialog(BaseDialog* dialog);

public:

  //
  // BaseDialog overrided methods
  //

  virtual BOOL onInitDialog();
  virtual BOOL onNotify(UINT controlID, LPARAM data);
  virtual BOOL onCommand(UINT controlID, UINT notificationID);
  virtual BOOL onDestroy() { return TRUE; }

  //
  // Helper methods
  //

  bool validateInput();
  void updateUI();
  void apply();

private:
  void initControls();
  void updateControlDependencies();

  //
  // Contol event handlers
  //

  void onAcceptRfbConnectionsClick();
  void onAcceptHttpConnectionsClick();
  void onAuthenticationClick();
  void onPrimaryPasswordChange();
  void onReadOnlyPasswordChange();
  void onUnsetPrimaryPasswordClick();
  void onUnsetReadOnlyPasswordClick();
  void onShowTrayIconCheckBoxClick();
  void onConnectToRdpCheckBoxClick();
  void onPollingIntervalSpinChangePos(LPNMUPDOWN message);
  void onRfbPortUpdate();
  void onHttpPortUpdate();
  void onUrlParamsClick();
  void onPollingIntervalUpdate();
  void onFileTransferCheckBoxClick();
  void onRemoveWallpaperCheckBoxClick();
  void onGrabTransparentWindowsChanged();
  void onUseD3DChanged();

  //
  // Input handling
  //

  void onBlockLocalInputChanged();
  void onBlockRemoteInputChanged();
  void onLocalInputPriorityChanged();
  void onInactivityTimeoutUpdate();
  void updateCheckboxesState();

protected:
  // Configuration
  ServerConfig* m_config;
  // Controls
  TextBox m_rfbPort;
  TextBox m_pollingInterval;
  CheckBox m_useMirrorDriver;
  CheckBox m_enableFileTransfers;
  CheckBox m_removeWallpaper;
  CheckBox m_acceptRfbConnections;
  CheckBox m_showTrayIcon;
  CheckBox m_connectToRdp;
  Control m_primaryPassword;
  Control m_readOnlyPassword;
  Control m_unsetPrimaryPassword;
  Control m_unsetReadOnlyPassword;
  CheckBox m_useAuthentication;
  SpinControl m_rfbPortSpin;
  SpinControl m_pollingIntervalSpin;

  //
  // Begin of input handling members
  //

  CheckBox m_blockRemoteInput;
  CheckBox m_blockLocalInput;
  CheckBox m_localInputPriority;
  TextBox m_localInputPriorityTimeout;
  SpinControl m_inactivityTimeoutSpin;

  //
  // End of input handling members
  //

  // Error notifications
  BaseDialog* m_parentDialog;

  // Primary password control.
  PasswordControl* m_ppControl;
  PasswordControl* m_vpControl;
};

