#pragma once
#include "Utils/CommonHeader.h"
#include "gui/BaseDialog.h"
#include "gui/Control.h"

class QueryConnectionDialog : public BaseDialog
{
public:
  static const int ACCEPT_CHOISE = 0x0;
  static const int REJECT_CHOISE = 0x1;

public:
  QueryConnectionDialog(const TCHAR* peerAddress, bool acceptByDefault,
    DWORD timeOutInSec);
  virtual ~QueryConnectionDialog();

protected:

  //
  // Inherited from BaseDialog.
  //

  virtual BOOL onInitDialog();
  virtual BOOL onNotify(UINT controlID, LPARAM data);
  virtual BOOL onCommand(UINT controlID, UINT notificationID);
  virtual BOOL onDestroy();
  virtual void onMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

  void initControls();

  void onAccept();
  void onReject();
  void onTimer();

  void updateTimeoutLabel();

protected:
  StringStorage m_peerAddress;
  bool m_acceptByDefault;
  DWORD m_timeout;

  Control m_peerAddressLabel;
  Control m_timeoutLabel;
  Control m_acceptButton;
  Control m_rejectButton;
};

