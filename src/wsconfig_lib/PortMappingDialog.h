#pragma once
#include "gui/BaseDialog.h"
#include "gui/Control.h"
#include "gui/ListBox.h"
#include "server_config_lib/Configurator.h"

class PortMappingDialog : public BaseDialog
{
public:
  PortMappingDialog();
  virtual ~PortMappingDialog();

  void setParentDialog(BaseDialog* dialog);

protected:

  //
  // Inherited from BaseDialog.
  //

  virtual BOOL onInitDialog();
  virtual BOOL onCommand(UINT controlID, UINT notificationID);
  virtual BOOL onNotify(UINT controlID, LPARAM data) { return TRUE; }
  virtual BOOL onDestroy() { return TRUE; }

  //
  // Controls event handlers.
  //

  void onAddButtonClick();
  void onEditButtonClick();
  void onRemoveButtonClick();
  void onExPortsListBoxSelChange();
  void onExPortsListBoxDoubleClick();

private:
  void initControls();

protected:
  ListBox m_exPortsListBox;
  Control m_editButton;
  Control m_removeButton;

  PortMappingContainer* m_extraPorts;

  BaseDialog* m_parent;
};

