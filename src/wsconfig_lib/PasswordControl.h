#pragma once
#include "gui/Control.h"
#include <vector>

class PasswordControl
{
public:
  PasswordControl(Control* changeButton, Control* unsetButton);
  virtual ~PasswordControl();
  void setEnabled(bool enabled);

  void unsetPassword(bool promtUser, HWND parentWindow);
  void setPassword(const TCHAR* plainText);
  void setCryptedPassword(const char* cryptedPass);
  bool hasPassword() const;
  const char* getCryptedPassword() const;

  bool showChangePasswordModalDialog(Control* parent);

private:
  void updateControlsState();
  void releaseCryptedPassword();

protected:
  Control* m_changeButton;
  Control* m_unsetButton;

  std::vector<char> m_cryptedPassword;

  bool m_enabled;
};

