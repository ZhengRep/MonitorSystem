#include "PortMappingDialog.h"
#include "EditPortMappingDialog.h"
#include "ConfigDialog.h"
#include "mnt_server/resource.h"

PortMappingDialog::PortMappingDialog()
  : BaseDialog(IDD_CONFIG_PORT_MAPPING_PAGE), m_parent(NULL)
{
}

PortMappingDialog::~PortMappingDialog()
{
}

void PortMappingDialog::setParentDialog(BaseDialog* dialog)
{
  m_parent = dialog;
}

void PortMappingDialog::initControls()
{
  HWND dialogHwnd = m_ctrlThis.getWindow();

  m_exPortsListBox.setWindow(GetDlgItem(dialogHwnd, IDC_MAPPINGS));
  m_editButton.setWindow(GetDlgItem(dialogHwnd, IDC_EDIT_PORT));
  m_removeButton.setWindow(GetDlgItem(dialogHwnd, IDC_REMOVE_PORT));
}

BOOL PortMappingDialog::onCommand(UINT controlID, UINT notificationID)
{
  switch (controlID) {
  case IDC_ADD_PORT:
    onAddButtonClick();
    break;
  case IDC_EDIT_PORT:
    onEditButtonClick();
    break;
  case IDC_REMOVE_PORT:
    onRemoveButtonClick();
    break;
  case IDC_MAPPINGS:
    switch (notificationID) {
    case LBN_SELCHANGE:
      onExPortsListBoxSelChange();
      break;
    case LBN_DBLCLK:
      onExPortsListBoxDoubleClick();
      break;
    }
    break;
  }
  return TRUE;
}

BOOL PortMappingDialog::onInitDialog()
{
  initControls();

  m_extraPorts = Configurator::getInstance()->getServerConfig()->getPortMappingContainer();

  //
  // Fill listbox.
  //

  StringStorage mappingString;

  for (size_t i = 0; i < m_extraPorts->count(); i++) {
    m_extraPorts->at(i)->toString(&mappingString);
    _ASSERT((int)i == i);
    m_exPortsListBox.insertString((int)i, mappingString.getString());
  }

  return TRUE;
}

void PortMappingDialog::onExPortsListBoxSelChange()
{
  int selectedIndex = m_exPortsListBox.getSelectedIndex();

  m_editButton.setEnabled(selectedIndex >= 0);
  m_removeButton.setEnabled(selectedIndex >= 0);
}

void PortMappingDialog::onAddButtonClick()
{
  EditPortMappingDialog addDialog(EditPortMappingDialog::Add);

  PortMapping newPM;

  addDialog.setMapping(&newPM);
  addDialog.setParent(&m_ctrlThis);

  if (addDialog.showModal() == IDOK) {
    {
      StringStorage mappingString;
      newPM.toString(&mappingString);
      m_exPortsListBox.addString(mappingString.getString());
    }

    m_extraPorts->pushBack(newPM);

    ((ConfigDialog*)m_parent)->updateApplyButtonState();
  }
}

void PortMappingDialog::onEditButtonClick()
{
  int selectedIndex = m_exPortsListBox.getSelectedIndex();

  if (selectedIndex == -1) {
    return;
  }

  PortMapping* pPM = m_extraPorts->at(selectedIndex);

  EditPortMappingDialog editDialog(EditPortMappingDialog::Edit);

  editDialog.setParent(&m_ctrlThis);
  editDialog.setMapping(pPM);

  if (editDialog.showModal() == IDOK) {
    StringStorage mappingString;
    pPM->toString(&mappingString);
    m_exPortsListBox.setItemText(selectedIndex, mappingString.getString());

    ((ConfigDialog*)m_parent)->updateApplyButtonState();
  }
}

void PortMappingDialog::onRemoveButtonClick()
{
  int selectedIndex = m_exPortsListBox.getSelectedIndex();

  if (selectedIndex == -1) {
    return;
  }

  m_exPortsListBox.removeString(selectedIndex);
  m_extraPorts->remove(selectedIndex);

  ((ConfigDialog*)m_parent)->updateApplyButtonState();

  //
  // Restore selection after item is removed.
  //

  if (m_exPortsListBox.getCount() > 0) {
    m_exPortsListBox.setSelectedIndex(selectedIndex);
    if (m_exPortsListBox.getSelectedIndex() == -1) {
      m_exPortsListBox.setSelectedIndex(selectedIndex - 1);
    }
    if (m_exPortsListBox.getSelectedIndex() == -1) {
      m_exPortsListBox.setSelectedIndex(selectedIndex + 1);
    }
  } else {
    m_removeButton.setEnabled(false);
    m_editButton.setEnabled(false);
  }
}

void PortMappingDialog::onExPortsListBoxDoubleClick()
{
  if (m_exPortsListBox.getSelectedIndex() != -1)
    onEditButtonClick();
}
