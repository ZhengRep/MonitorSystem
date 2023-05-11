#include "WindowsClipboard.h"
#include "vnc_server_app/NamingDefs.h"

const HINSTANCE WindowsClipboard::m_hinst = GetModuleHandle(0);

WindowsClipboard::WindowsClipboard(ClipboardListener* clipboardListener, LogWriter* log)
  :MessageWindow(m_hinst, ClipboardNames::CLIPBOARD_WIN_CLASS_NAME),
  m_hwndNextViewer(0),
  m_clipboardListener(clipboardListener),
  m_log(log)
{
  resume();
}

WindowsClipboard::~WindowsClipboard(void)
{
  terminate();
  wait();
}

bool WindowsClipboard::writeToClipBoard(const TCHAR* text)
{
  StringStorage clipboard;
  convertFromRfbFormat(text, &clipboard);

  if (OpenClipboard(m_hwnd)) {
    EmptyClipboard();

    size_t clipSize = clipboard.getSize();
    HGLOBAL hglb = GlobalAlloc(GMEM_MOVEABLE, clipSize);
    if (hglb) {
      TCHAR* buff = (TCHAR*)GlobalLock(hglb);
      memcpy(buff, clipboard.getString(), clipSize);
      GlobalUnlock(hglb);

#ifdef _UNICODE
      SetClipboardData(CF_UNICODETEXT, hglb);
#else
      SetClipboardData(CF_TEXT, hglb);

      // Store locale information in the clipboard as well.
      HGLOBAL hmemLocale = GlobalAlloc(GMEM_MOVEABLE, sizeof(LCID));
      if (hmemLocale != NULL) {
        LCID* pLocale = (LCID*)GlobalLock(hmemLocale);
        *pLocale = GetSystemDefaultLCID(); // or maybe GetUserDefaultLCID()?
        GlobalUnlock(hmemLocale);
        if (SetClipboardData(CF_LOCALE, hmemLocale) == NULL) {
          GlobalFree(hmemLocale);
        }
      }
#endif
    }

    CloseClipboard();
  }
  return false;
}

void WindowsClipboard::readFromClipBoard(StringStorage* clipDest) const
{
#ifdef _UNICODE
  const UINT CF_TCTEXT = CF_UNICODETEXT;
#else
  const UINT CF_TCTEXT = CF_TEXT;
#endif

  clipDest->setString(_T(""));
  if (!IsClipboardFormatAvailable(CF_TCTEXT) || !OpenClipboard(m_hwnd)) {
    return;
  }

  HANDLE hglb = GetClipboardData(CF_TCTEXT);
  if (hglb != NULL) {
    const TCHAR* lpstr = (const TCHAR*)GlobalLock(hglb);
    if (lpstr != 0) {
      clipDest->setString(lpstr);
      GlobalUnlock(hglb);
    }
  }
  CloseClipboard();
}

bool WindowsClipboard::wndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  int fake = 3;
  switch (message)
  {
  case WM_CREATE:
    m_hwndNextViewer = SetClipboardViewer((HWND)wParam);
    break;

  case WM_CHANGECBCHAIN:
    if ((HWND)wParam == m_hwndNextViewer) {
      m_hwndNextViewer = (HWND)lParam;
    } else if (m_hwndNextViewer != NULL) {
      SendMessage(m_hwndNextViewer, message, wParam, lParam);
    }

    break;

  case WM_DESTROY:
    ChangeClipboardChain(m_hwnd, m_hwndNextViewer);
    break;

  case WM_DRAWCLIPBOARD:  // clipboard contents changed.
  {
    StringStorage winClip, rfbClip;
    readFromClipBoard(&winClip);
    convertToRfbFormat(&winClip, &rfbClip);

    m_clipboardListener->onClipboardUpdate(&rfbClip);
  }
  SendMessage(m_hwndNextViewer, message, wParam, lParam);
  break;

  default:
    return false; // Message not processing
  }

  return true;
}

void WindowsClipboard::execute()
{
  m_log->info(_T("clipboard thread id = %d"), getThreadId());

  if (!createWindow()) {
    return;
  }

  MSG msg;
  while (!isTerminating()) {
    if (GetMessage(&msg, m_hwnd, 0, 0)) {
      DispatchMessage(&msg);
    } else {
      break;
    }
  }

  destroyWindow();
}

void WindowsClipboard::onTerminate()
{
  if (m_hwnd != 0) {
    PostMessage(m_hwnd, WM_QUIT, 0, 0);
  }
}

void WindowsClipboard::convertToRfbFormat(const StringStorage* source, StringStorage* dest)
{
  const TCHAR* srcText = source->getString();
  size_t length = source->getLength();
  TCHAR* rfbText = new TCHAR[length + 1];

  size_t j = 0;
  for (size_t i = 0; i < length; i++) {
    if (!(srcText[i] == 0x0d && srcText[i + 1] == 0x0a)) {
      rfbText[j] = srcText[i];
      j++;
    }
  }
  rfbText[j] = 0;
  dest->setString(rfbText);
  delete[] rfbText;
}

void WindowsClipboard::convertFromRfbFormat(const TCHAR* source, StringStorage* dest)
{
  size_t lfCount = 0;
  size_t sourceLen = _tcslen(source);
  for (size_t i = 0; i < sourceLen; i++) {
    if (source[i] == 0x0a) {
      lfCount++;
    }
  }

  size_t destLen = sourceLen + lfCount;
  TCHAR* destText = new TCHAR[destLen + 1];
  int j = 0;
  for (size_t i = 0; i < sourceLen; i++) {
    if (source[i] == 0x0a) {
      destText[j] = 0x0d;
      j++;
    }
    destText[j] = source[i];
    j++;
  }
  destText[j] = 0;

  dest->setString(destText);
  delete[] destText;
}
