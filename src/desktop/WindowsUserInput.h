#pragma once
#include "UserInput.h"
#include "WindowsClipboard.h"
#include "win_system/WindowsDisplays.h"
#include "win_system/InputInjector.h"
#include "log_writer/LogWriter.h"
#include "utils/Keymap.h"

class WindowsUserInput: public UserInput
{
public:
  WindowsUserInput(ClipboardListener* clipboardListener, bool ctrlAltDelEnabled, LogWriter* log);
  virtual ~WindowsUserInput(void);

  virtual void setNewClipboard(const StringStorage* newClipboard);
  virtual void setMouseEvent(const Point* newPos, UINT8 keyFlag);
  virtual void setKeyboardEvent(UINT32 keySym, bool down);

  virtual void getCurrentUserInfo(StringStorage* desktopName, StringStorage* userName);
  virtual void getDisplayNumberCoords(Rect* rect, unsigned char dispNumber);
  virtual void getNormalizedRect(Rect* rect);
  virtual void getPrimaryDisplayCoords(Rect* rect);
  virtual void getWindowCoords(HWND hwnd, Rect* rect);
  virtual HWND getWindowHandleByName(const StringStorage* windowName);
  virtual void getApplicationRegion(unsigned int procId, Region* region);
  virtual bool isApplicationInFocus(unsigned int procId);

  virtual void initKeyFlag(UINT8 initValue) { m_prevKeyFlag = initValue; }

protected:
  void toFbCoordinates(Rect* rect);

  WindowsClipboard* m_clipboard;
  WindowsDisplays m_winDisplays;

  Keymap m_keyMap;
  InputInjector m_inputInjector;

  UINT8 m_prevKeyFlag;

  LogWriter* m_log;
};

