#include "ControlAuthDialog.h"
#include "mnt_server/resource.h"
#include "server_config_lib/ServerConfig.h"

ControlAuthDialog::ControlAuthDialog()
  : BaseDialog(IDD_CONTROL_AUTH)
{
}

ControlAuthDialog::~ControlAuthDialog()
{
}

const TCHAR* ControlAuthDialog::getPassword() const
{
  return m_password.getString();
}

BOOL ControlAuthDialog::onInitDialog()
{
  m_password.setString(_T(""));

  HWND hwnd = m_ctrlThis.getWindow();
  m_passwordTextBox.setWindow(GetDlgItem(hwnd, IDC_PASSWORD_EDIT));
  m_passwordTextBox.setTextLengthLimit(ServerConfig::MNT_PASSWORD_SIZE);

  SetForegroundWindow(hwnd);
  m_passwordTextBox.setFocus();

  return TRUE;
}

BOOL ControlAuthDialog::onNotify(UINT controlID, LPARAM data)
{
  return TRUE;
}

BOOL ControlAuthDialog::onCommand(UINT controlID, UINT notificationID)
{
  switch (controlID) {
  case IDOK:
    m_passwordTextBox.getText(&m_password);
    kill(controlID);
    break;
  case IDCANCEL:
    kill(controlID);
    break;
  }
  return TRUE;
}

BOOL ControlAuthDialog::onDestroy()
{
  return TRUE;
}
