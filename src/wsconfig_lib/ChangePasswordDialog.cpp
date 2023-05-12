#include "ChangePasswordDialog.h"
#include "mnt_server/resource.h"
#include "server_config_lib/ServerConfig.h"
#include "utils/MntPassCrypt.h"
#include "utils/AnsiStringStorage.h"

ChangePasswordDialog::ChangePasswordDialog(Control* parent, bool isNewPassword)
  : BaseDialog(IDD_CHANGE_PASSWORD), m_allowEmptyPassword(false), m_newPassword(isNewPassword)
{
  setParent(parent);

  m_passwordEmptyTooltip.setText(StringTable::getString(IDS_PASSWORD_IS_EMPTY));
  m_passwordEmptyTooltip.setTitle(StringTable::getString(IDS_MBC_MNTCONTROL));

  m_passwordsNotMatchTooltip.setText(StringTable::getString(IDS_PASSWORDS_NOT_MATCH));
  m_passwordsNotMatchTooltip.setTitle(StringTable::getString(IDS_MBC_MNTCONTROL));

  m_passwordWeakTooltip.setText(StringTable::getString(IDS_BAD_PASSWORD));
  m_passwordWeakTooltip.setTitle(StringTable::getString(IDS_MBC_BAD_PASSWORD));
}

ChangePasswordDialog::~ChangePasswordDialog()
{
}

const TCHAR* ChangePasswordDialog::getPasswordInPlainText() const
{
  return m_passwordText.getString();
}

BOOL ChangePasswordDialog::onInitDialog()
{
  initControls();
  if (m_newPassword) {
    m_ctrlThis.setText(StringTable::getString(IDS_NEW_PASSWORD));
  } else {
    m_ctrlThis.setText(StringTable::getString(IDS_CHANGE_PASSWORD));
  }
  return TRUE;
}

BOOL ChangePasswordDialog::onCommand(UINT cID, UINT nID)
{
  if (nID == BN_CLICKED) {
    switch (cID) {
    case IDOK:
      onOkButtonClick();
      break;
    case IDCANCEL:
      onCancelButtonClick();
      break;
    }
  }
  return TRUE;
}

void ChangePasswordDialog::onOkButtonClick()
{
  StringStorage password1;
  StringStorage password2;
  m_password1.getText(&password1);
  m_password2.getText(&password2);

  if (password1.isEmpty() && !m_allowEmptyPassword) {
    m_password1.showBalloonTip(&m_passwordEmptyTooltip);
    m_password1.setFocus();
    return;
  }
  if (!password1.isEqualTo(&password2)) {
    m_password2.showBalloonTip(&m_passwordsNotMatchTooltip);
    m_password2.setFocus();
    return;
  }

  if (!AnsiStringStorage::checkAnsiConversion(password1)) {
    m_password1.showBalloonTip(&m_passwordWeakTooltip);
    m_password1.setFocus();
    return;
  }

  m_passwordText.setString(password1.getString());

  kill(IDOK);
}

void ChangePasswordDialog::onCancelButtonClick()
{
  kill(IDCANCEL);
}

void ChangePasswordDialog::initControls()
{
  HWND hwnd = m_ctrlThis.getWindow();
  m_password1.setWindow(GetDlgItem(hwnd, IDC_PASSWORD));
  m_password2.setWindow(GetDlgItem(hwnd, IDC_PASSWORD2));

  m_password1.setTextLengthLimit(MntPassCrypt::MNT_PASSWORD_SIZE);
  m_password2.setTextLengthLimit(MntPassCrypt::MNT_PASSWORD_SIZE);
}
