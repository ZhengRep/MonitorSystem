#include "WindowsUserInput.h"
#include "thread/DesktopSelector.h"
#include "winSystem/Environment.h"
#include "winSystem/Keyboard.h"
#include "gui/WindowFinder.h"
#include "Utils/BrokenHandleException.h"
#include "Utils/Macros.h"

WindowsUserInput::WindowsUserInput(ClipboardListener* clipboardListener, bool ctrlAltDelEnabled, LogWriter* log)
  : m_prevKeyFlag(0),
    m_inputInjector(ctrlAltDelEnabled, log),
    m_log(log)
{
  m_clipboard = new WindowsClipboard(clipboardListener, m_log);
}

WindowsUserInput::~WindowsUserInput(void)
{
  delete m_clipboard;
}

void WindowsUserInput::setNewClipboard(const StringStorage* newClipboard)
{
  m_clipboard->writeToClipBoard(newClipboard->getString());
}

void WindowsUserInput::setMouseEvent(const Point* newPos, UINT8 keyFlag)
{
  if (GetSystemMetrics(SM_SWAPBUTTON))
  {
    // read values of first and third bytes..
    UINT8 left = keyFlag & 1;
    UINT8 right = keyFlag & 4;
    // set them to zero..
    keyFlag &= 0xFA;
    // and set swapped values
    keyFlag |= (right & 4) >> 2;
    keyFlag |= left << 2;
  }

  DWORD dwFlags = 0;
  dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

  bool prevState;
  bool currState;
  // Check the left button on change state
  prevState = (m_prevKeyFlag & 1) != 0;
  currState = (keyFlag & 1) != 0;
  if (currState != prevState) {
    if (currState) {
      dwFlags |= MOUSEEVENTF_LEFTDOWN;
    } else {
      dwFlags |= MOUSEEVENTF_LEFTUP;
    }
  }
  // Check the middle button on change state
  prevState = (m_prevKeyFlag & 2) != 0;
  currState = (keyFlag & 2) != 0;
  if (currState != prevState) {
    if (currState) {
      dwFlags |= MOUSEEVENTF_MIDDLEDOWN;
    } else {
      dwFlags |= MOUSEEVENTF_MIDDLEUP;
    }
  }
  // Check the right button on change state
  prevState = (m_prevKeyFlag & 4) != 0;
  currState = (keyFlag & 4) != 0;
  if (currState != prevState) {
    if (currState) {
      dwFlags |= MOUSEEVENTF_RIGHTDOWN;
    } else {
      dwFlags |= MOUSEEVENTF_RIGHTUP;
    }
  }

  // Check on a mouse wheel
  DWORD mouseWheelValue = 0;
  bool prevWheelUp = (m_prevKeyFlag & 8) != 0;
  bool currWheelUp = (keyFlag & 8) != 0;
  bool prevWheelDown = (m_prevKeyFlag & 16) != 0;
  bool currWheelDown = (keyFlag & 16) != 0;

  if (currWheelUp && !prevWheelUp) {
    dwFlags |= MOUSEEVENTF_WHEEL;
    mouseWheelValue = 120;
  } else if (currWheelDown && !prevWheelDown) {
    dwFlags |= MOUSEEVENTF_WHEEL;
    mouseWheelValue = -120;
  }

  m_prevKeyFlag = keyFlag;

  // Normilize pointer position
  UINT16 desktopWidth = GetSystemMetrics(SM_CXSCREEN);
  UINT16 desktopHeight = GetSystemMetrics(SM_CYSCREEN);
  int fbOffsetX = GetSystemMetrics(SM_XVIRTUALSCREEN);
  int fbOffsetY = GetSystemMetrics(SM_YVIRTUALSCREEN);
  INT32 x = (INT32)((newPos->x + fbOffsetX) * 65535 / (desktopWidth - 1));
  INT32 y = (INT32)((newPos->y + fbOffsetY) * 65535 / (desktopHeight - 1));

  INPUT input;
  memset(&input, 0, sizeof(INPUT));
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = dwFlags;
  input.mi.dx = x;
  input.mi.dy = y;
  input.mi.mouseData = mouseWheelValue;
  SendInput(1, &input, sizeof(INPUT));
  DWORD error = GetLastError();
}

void WindowsUserInput::setKeyboardEvent(UINT32 keySym, bool down)
{
  try {
    m_log->info(_T("Received the %#4.4x keysym, down = %d"), keySym, (int)down);
    // Generate single key event.
    BYTE vkCode;
    WCHAR ch;
    bool release = !down;
    bool extended;

    if (m_keyMap.keySymToVirtualCode(keySym, &vkCode, &extended)) {
      m_inputInjector.injectKeyEvent(vkCode, release, extended);
    } else if (m_keyMap.keySymToUnicodeChar(keySym, &ch)) {
      m_inputInjector.injectCharEvent(ch, release);
    } else {
      StringStorage message;
      message.format(_T("Unknown %d keysym"), keySym);
      throw Exception(message.getString());
    }
  } catch (Exception& someEx) {
    m_log->error(_T("Exception while processing key event: %s"), someEx.getMessage());
  }
}

void WindowsUserInput::getCurrentUserInfo(StringStorage* desktopName, StringStorage* userName)
{
  if (!DesktopSelector::getCurrentDesktopName(desktopName) &&
    !Environment::getCurrentUserName(userName, m_log)) {
    StringStorage errMess;
    Environment::getErrStr(_T("Can't get current user info"), &errMess);
    throw Exception(errMess.getString());
  }
}

void WindowsUserInput::getDisplayNumberCoords(Rect* rect, unsigned char dispNumber)
{
  m_winDisplays.getDisplayCoordinates(dispNumber, rect);
}

void WindowsUserInput::getNormalizedRect(Rect* rect)
{
  toFbCoordinates(rect);
}

void WindowsUserInput::getPrimaryDisplayCoords(Rect* rect)
{
  rect->left = 0;
  rect->top = 0;
  rect->right = GetSystemMetrics(SM_CXSCREEN);
  rect->bottom = GetSystemMetrics(SM_CYSCREEN);
  rect->move(-GetSystemMetrics(SM_XVIRTUALSCREEN),
    -GetSystemMetrics(SM_YVIRTUALSCREEN));
}

void WindowsUserInput::getWindowCoords(HWND hwnd, Rect* rect)
{
  rect->clear();
  RECT winRect;
  if (GetWindowRect(hwnd, &winRect)) {
    rect->fromWindowsRect(&winRect);
    rect->move(-GetSystemMetrics(SM_XVIRTUALSCREEN),
      -GetSystemMetrics(SM_YVIRTUALSCREEN));
  } else {
    StringStorage errMess;
    Environment::getErrStr(_T("Can't get window coordinates"), &errMess);
    throw BrokenHandleException(errMess.getString());
  }
}

HWND WindowsUserInput::getWindowHandleByName(const StringStorage* windowName)
{
  return WindowFinder::findFirstWindowByName(*windowName);
}

void WindowsUserInput::getApplicationRegion(unsigned int procId, Region* region)
{
  region->clear();
  HWND hForegr = GetWindow(GetForegroundWindow(), GW_HWNDLAST);

  RECT winRect;
  Rect rect;
  while (hForegr != NULL) {
    GetWindowRect(hForegr, &winRect);

    DWORD style = GetWindowLong(hForegr, GWL_STYLE);
    DWORD procForegr;
    GetWindowThreadProcessId(hForegr, &procForegr);
    if (style & WS_VISIBLE) {
      rect.fromWindowsRect(&winRect);
      if (procForegr == procId) {
        region->addRect(&rect);
      } else {
        region->subtract(&unmove(Region(&rect)));
      }
    }
    hForegr = GetWindow(hForegr, GW_HWNDPREV);
  }
  region->translate(-GetSystemMetrics(SM_XVIRTUALSCREEN), -GetSystemMetrics(SM_YVIRTUALSCREEN));
}

bool WindowsUserInput::isApplicationInFocus(unsigned int procId)
{
  HWND hKeyboardInputWindow = GetForegroundWindow();
  if (hKeyboardInputWindow == NULL) {
    return false;
  }

  DWORD procForeground;
  GetWindowThreadProcessId(hKeyboardInputWindow, &procForeground);

  return (procForeground == (DWORD)procId);
}

void WindowsUserInput::toFbCoordinates(Rect* rect)
{
  rect->move(-GetSystemMetrics(SM_XVIRTUALSCREEN), -GetSystemMetrics(SM_YVIRTUALSCREEN));
}
