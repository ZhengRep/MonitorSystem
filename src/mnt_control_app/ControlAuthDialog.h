#pragma once
#include "gui/BaseDialog.h"
#include "gui/TextBox.h"

class ControlAuthDialog : public BaseDialog
{
public:
  ControlAuthDialog();
  virtual ~ControlAuthDialog();


  const TCHAR* getPassword() const;
protected:
  virtual BOOL onInitDialog();
  virtual BOOL onNotify(UINT controlID, LPARAM data);
  virtual BOOL onCommand(UINT controlID, UINT notificationID);
  virtual BOOL onDestroy();
private:
  StringStorage m_password;
  TextBox m_passwordTextBox;
};

