#pragma once
#include "gui/BaseDialog.h"
#include "gui/TextBox.h"
#include "gui/BalloonTip.h"
#include "utils/StringStorage.h"

class ChangePasswordDialog : public BaseDialog
{
public:
  ChangePasswordDialog(Control* parent, bool isNewPassword);
  virtual ~ChangePasswordDialog();
  const TCHAR* getPasswordInPlainText() const;
protected:

  virtual BOOL onInitDialog();
  virtual BOOL onCommand(UINT cID, UINT nID);
  virtual BOOL onNotify(UINT controlID, LPARAM data) { return TRUE; }
  virtual BOOL onDestroy() { return TRUE; }


  void onOkButtonClick();
  void onCancelButtonClick();

private:
  void initControls();

protected:
  // Controls
  TextBox m_password1;
  TextBox m_password2;
  // Password
  StringStorage m_passwordText;
  // Tooltip
  BalloonTip m_passwordEmptyTooltip;
  BalloonTip m_passwordsNotMatchTooltip;
  BalloonTip m_passwordWeakTooltip;
  // Helper members
  bool m_newPassword;
  bool m_allowEmptyPassword;
};

