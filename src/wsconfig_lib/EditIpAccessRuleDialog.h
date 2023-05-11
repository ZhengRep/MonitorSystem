#pragma once
#include "gui/BaseDialog.h"
#include "gui/TextBox.h"
#include "gui/CheckBox.h"
#include "gui/BalloonTip.h"
#include "server_config_lib/IpAccessRule.h"
#include "Utils/StringStorage.h"

class EditIpAccessRuleDialog : public BaseDialog
{
public:
  EditIpAccessRuleDialog();
  virtual ~EditIpAccessRuleDialog();

  void setIpAccessControl(IpAccessRule* iac) { m_data = iac; }
  IpAccessRule* getIpAccessControl() { return m_data; }

  void setEditFlag(bool flagEnabled);

protected:

  //
  // BaseDialog overrided methods
  //

  virtual BOOL onInitDialog();
  virtual BOOL onCommand(UINT cID, UINT nID);
  virtual BOOL onNotify(UINT controlID, LPARAM data) { return TRUE; }
  virtual BOOL onDestroy() { return TRUE; }

  //
  // Controls event handlers
  //

  void onOkButtonClick();
  void onCancelButtonClick();
  void onAccessTypeRadioClick(int num);
private:
  void initControls();
  bool validateInput();

protected:
  // Controls
  TextBox m_firstIp;
  TextBox m_lastIp;
  CheckBox m_access[3];
  // Data
  IpAccessRule* m_data;
  bool m_isOpenedForEdit;

  BalloonTip m_warningBalloonTip;
  BalloonTip m_lastIpLessThanFirstBT;
};

