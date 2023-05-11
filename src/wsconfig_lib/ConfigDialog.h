#pragma once
#include "config_lib/SettingsManager.h"
#include "config_lib/RegistrySettingsManager.h"
#include "gui/BaseDialog.h"
#include "gui/Control.h"
#include "gui/ListBox.h"
#include "gui/TabControl.h"
#include "server_config_lib/Configurator.h"
#include "ServerConfigDialog.h"
#include "AdministrationConfigDialog.h"
#include "PortMappingDialog.h"
#include "IpAccessControlDialog.h"
#include "mnt_control_app/ControlCommand.h"

class ConfigDialog : public BaseDialog
{
public:
  ConfigDialog(bool forService, ControlCommand* reloadConfigCommand);
  ConfigDialog(bool forService);
  ConfigDialog();
  virtual ~ConfigDialog();

  void updateApplyButtonState();

  void setConfigReloadCommand(ControlCommand* command);
  void setServiceFlag(bool serviceFlag);

  bool isConfiguringService();

protected:

  void initControls();
  void loadSettings();
  // Init dialog handler
  virtual BOOL onInitDialog();
  virtual BOOL onCommand(UINT controlID, UINT notificationID);
  virtual BOOL onNotify(UINT controlID, LPARAM data);
  virtual BOOL onDestroy();
  // Button handlers
  void onCancelButtonClick();
  void onOKButtonClick();
  void onApplyButtonClick();
  // Tab handlers
  void onTabChange();
  void onTabChanging();
private:
  void moveDialogToTabControl(BaseDialog* dialog);
  bool validateInput();
  void updateCaption();
protected:
  // Controls
  Control m_ctrlApplyButton;
  TabControl m_tabControl;
  // Settings
  Configurator* m_config;
  // Dialogs for tab control
  ServerConfigDialog m_serverConfigDialog;
  PortMappingDialog m_portMappingDialog;
  AdministrationConfigDialog m_administrationConfigDialog;
  IpAccessControlDialog m_ipAccessControlDialog;
  // Other members
  bool m_isConfiguringService;

  ControlCommand* m_reloadConfigCommand;

  int m_lastSelectedTabIndex;
};


