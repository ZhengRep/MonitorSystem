#pragma once
#include "utils/CommonHeader.h"
#include "log_writer/LogWriter.h"
#include "SystemException.h"

class InputInjector
{
public:
  InputInjector(bool ctrlAltDelEnabled, LogWriter* log);
  ~InputInjector();
  void resetModifiers();
  void injectKeyPress(BYTE vkCode);
  void injectKeyRelease(BYTE vkCode);
  void injectKeyEvent(BYTE vkCode, bool release, bool extended = false);
  void injectCharEvent(WCHAR ch, bool release) throw(SystemException);

private:
  bool capsToggled();
  bool isDeadKey(SHORT scanResult, HKL keyboardLayout);
  bool isOneKeyEventChar(WCHAR ch, SHORT scanResult, HKL keyboardLayout);
  bool isDifferentWith(BYTE modifier, BYTE modStateValueOfOn, BYTE virtKey, HKL keyboardLayout);
  bool isInvariantToShift(BYTE virtKey, HKL keyboardLayout);
  bool isResistantToCaps(BYTE virtKey, HKL keyboardLayout);
  bool isAscii(WCHAR ch);
  HKL getCurrentKbdLayout();
  SHORT searchVirtKey(WCHAR ch, HKL hklCurrent);
  /**
   * Array of extended virtual codes.
   */
  static const BYTE EXTENDED_KEYS[];

  bool m_controlIsPressed;
  bool m_menuIsPressed;
  bool m_deleteIsPressed;
  bool m_shiftIsPressed;
  bool m_winIsPressed;
  bool m_ctrlAltDelEnabled;

  LogWriter* m_log;
};

