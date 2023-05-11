#pragma once
#include "gui/BaseDialog.h"
#include "gui/CheckBox.h"
#include "gui/TextBox.h"
#include "gui/SpinControl.h"
#include "gui/BalloonTip.h"
#include "server_config_lib/ServerConfig.h"
#include "PasswordControl.h"

class AdministrationConfigDialog : public BaseDialog
{
public:
  AdministrationConfigDialog();
  virtual ~AdministrationConfigDialog();
public:

  void setParentDialog(BaseDialog* dialog);

  //
  // BaseDialog overrided methods
  //

  virtual BOOL onInitDialog();
  virtual BOOL onCommand(UINT controlID, UINT notificationID);
  virtual BOOL onDestroy() { return TRUE; }
  virtual BOOL onNotify(UINT controlID, LPARAM data) { return TRUE; }

  //
  // Helper methods
  //

  bool validateInput();
  void updateUI();
  void apply();

private:
  void initControls();

  //
  // Control event handlers
  //

  void onShareRadioButtonClick(int number);
  void onOpenFolderButtonClick();
  void onLogLevelUpdate();
  void onDARadioButtonClick(int number);
  void onLogForAllUsersClick();
  void onUseControlAuthClick();
  void onRepeatControlAuthClick();
  void onChangeControlPasswordClick();
  void onUnsetControlPasswordClick();

  //
  // Helper methods
  //

  void getFolderName(const TCHAR* key, StringStorage* folder);

protected:
  // Configuration
  ServerConfig* m_config;
  // Controls
  Control m_openLogPathButton;
  Control m_setControlPasswordButton;
  Control m_unsetControlPasswordButton;
  CheckBox m_disconnectAction[3];
  TextBox m_logLevel;
  TextBox m_logPathTB;
  CheckBox m_useControlAuth;
  CheckBox m_repeatControlAuth;
  CheckBox m_shared[5];
  CheckBox m_logForAllUsers;
  SpinControl m_logSpin;
  BaseDialog* m_parentDialog;

  PasswordControl* m_cpControl;

  friend class ConfigDialog;
};

