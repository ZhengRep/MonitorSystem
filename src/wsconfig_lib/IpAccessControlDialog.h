#pragma once
#include "gui/BaseDialog.h"
#include "gui/ListView.h"
#include "gui/SpinControl.h"
#include "gui/BalloonTip.h"
#include "server_config_lib/Configurator.h"
#include "EditIpAccessRuleDialog.h"

class IpAccessControlDialog : public BaseDialog
{
public:
  IpAccessControlDialog();
  virtual ~IpAccessControlDialog();

  void setParentDialog(BaseDialog* dialog);

  //
  // BaseDialog overrided methods
  //

  virtual BOOL onInitDialog();
  virtual BOOL onCommand(UINT controlID, UINT notificationID);
  virtual BOOL onNotify(UINT controlID, LPARAM data);
  virtual BOOL onDestroy() { return TRUE; }

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

  void onAddButtonClick();
  void onEditButtonClick();
  void onRemoveButtonClick();
  void onMoveUpButtonClick();
  void onMoveDownButtonClick();
  void onListViewSelChange();
  void onListViewSelChangeDblClick();
  void onAcceptRadioClick();
  void onRefuseRadioClick();
  void onAllowLoopbackConnectionsClick();
  void onAllowOnlyLoopbackConnectionsClick();
  void onIpCheckUpdate();
  void onQueryTimeoutUpdate();

  //
  // Private helper methods
  //

  void updateButtonsState();
  void updateCheckBoxesState();
  void setListViewItemText(int index, IpAccessRule* control);

private:
  // Configuration
  IpAccessControl* m_container;
  ServerConfig* m_config;
  // Child dialog
  EditIpAccessRuleDialog m_editDialog;
  // Controls
  ListView m_list;
  Control m_addButton;
  Control m_editButton;
  Control m_removeButton;
  Control m_moveUpButton;
  Control m_moveDownButton;
  CheckBox m_defaultActionAccept;
  CheckBox m_defaultActionRefuse;
  CheckBox m_allowLoopbackConnections;
  CheckBox m_onlyLoopbackConnections;
  TextBox m_queryTimeout;
  TextBox m_ip;
  Control m_ipCheckResult;
  SpinControl m_queryTimeoutSpin;
  BaseDialog* m_parentDialog;
};

