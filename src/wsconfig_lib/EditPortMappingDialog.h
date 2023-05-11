#pragma once
#include "gui/BaseDialog.h"
#include "gui/TextBox.h"
#include "server_config_lib/PortMapping.h"

class EditPortMappingDialog : public BaseDialog
{
public:
  typedef enum {
    Add = 0x0,
    Edit = 0x1
  } DialogType;

public:
  EditPortMappingDialog(DialogType dlgType);
  virtual ~EditPortMappingDialog();

public:
  void setMapping(PortMapping* mapping);
protected:
  void initControls();
  bool isUserDataValid();

  //
  // Inherited from BaseDialog
  //

  virtual BOOL onInitDialog();
  virtual BOOL onCommand(UINT cID, UINT nID);
  virtual BOOL onNotify(UINT controlID, LPARAM data) { return TRUE; }
  virtual BOOL onDestroy() { return TRUE; }

  void onOkButtonClick();
  void onCancelButtonClick();
protected:
  TextBox m_geometryTextBox;
  TextBox m_portTextBox;
  DialogType m_dialogType;
  PortMapping* m_mapping;
};

