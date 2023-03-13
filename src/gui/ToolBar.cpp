#include "ToolBar.h"

ToolBar::ToolBar()
{
  INITCOMMONCONTROLSEX initCtrlEx;

  initCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
  initCtrlEx.dwICC = ICC_BAR_CLASSES;
  InitCommonControlsEx(&initCtrlEx);

  m_hWndToolbar = 0;
  m_initialStr = -1;
}

ToolBar::~ToolBar()
{
  if (m_hWndToolbar) {
    DestroyWindow(m_hWndToolbar);
  }
}

void ToolBar::setViewAutoButtons(int iButton, int style)
{
  m_autoButtons[iButton] = style;
}

void ToolBar::loadToolBarfromRes(DWORD id)
{
  BITMAP bmp;

  HBITMAP hbmp = LoadBitmap(GetModuleHandle(NULL),
    MAKEINTRESOURCE(id));
  GetObject(hbmp, sizeof(BITMAP), &bmp);
  m_width = bmp.bmWidth;
  m_height = bmp.bmHeight;
  m_numberTB = m_width / m_height;
  m_id = id;
  DeleteObject(hbmp);
}

void ToolBar::setButtonsRange(DWORD id)
{
  m_initialStr = id;
}

void ToolBar::attachToolBar(HWND hwnd)
{
  std::vector<TBBUTTON> tbuttons;

  for (int i = 0; i < m_numberTB; i++) {
    TBBUTTON tbutton;

    ZeroMemory(&tbutton, sizeof(tbutton));
    if (m_autoButtons.find(i) != m_autoButtons.end()) {
      switch (m_autoButtons[i]) {
      case TB_Style_sep:
        tbutton.fsStyle = TBSTYLE_SEP;
        tbuttons.push_back(tbutton);
        break;
      case TB_Style_gap:
        tbutton.iBitmap = I_IMAGENONE;
        tbuttons.push_back(tbutton);
        break;
      }
    }
    tbutton.iBitmap = i;
    tbutton.idCommand = m_initialStr == 0 ? 0 : m_initialStr + i;
    tbutton.fsState = TBSTATE_ENABLED;
    tbutton.fsStyle = TBSTYLE_BUTTON;
    tbuttons.push_back(tbutton);
  }
  m_autoButtons.clear();

  m_hWndToolbar = CreateToolbarEx(hwnd,
    WS_VISIBLE | WS_CHILD | TBSTYLE_TOOLTIPS | WS_CLIPSIBLINGS | TBSTYLE_FLAT | WS_BORDER,
    m_id,
    static_cast<int>(tbuttons.size()),
    GetModuleHandle(NULL),
    m_id,
    &tbuttons.front(),
    static_cast<int>(tbuttons.size()),
    0, 0, 0, 0,
    sizeof(TBBUTTON));
  SendMessage(m_hWndToolbar, TB_SETINDENT, 4, 0);
}

bool ToolBar::create(int _tbID, HWND _parentHwnd, DWORD dwStyle)
{
  dwStyle |= WS_CHILD;
  _ASSERT(m_hWndToolbar == 0);
  m_hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, 0, dwStyle, 0, 0, 0, 0, _parentHwnd, reinterpret_cast<HMENU>(_tbID), GetModuleHandle(0), 0);
  if (m_hWndToolbar) {
    SendMessage(m_hWndToolbar, TB_BUTTONSTRUCTSIZE, static_cast<WPARAM>(sizeof(TBBUTTON)), 0);
  }
  return !!m_hWndToolbar;
}

LRESULT ToolBar::addBitmap(int nButtons, UINT bitmapID)
{
  TBADDBITMAP resBitmap;
  resBitmap.hInst = GetModuleHandle(0);
  resBitmap.nID = bitmapID;
  return SendMessage(m_hWndToolbar, TB_ADDBITMAP, nButtons, reinterpret_cast<LPARAM>(&resBitmap));
}

LRESULT ToolBar::addSystemBitmap(UINT stdBitmapID)
{
  TBADDBITMAP resBitmap;
  resBitmap.hInst = HINST_COMMCTRL;
  resBitmap.nID = stdBitmapID;
  return SendMessage(m_hWndToolbar, TB_ADDBITMAP, 0, (LPARAM)&resBitmap);
}

bool ToolBar::addNButton(int nButtons, LPTBBUTTON tbb)
{
  LRESULT result = SendMessage(m_hWndToolbar, TB_ADDBUTTONS, nButtons, reinterpret_cast<LPARAM>(tbb));
  if (result) {
    SendMessage(m_hWndToolbar, TB_AUTOSIZE, 0, 0);
  }
  return !!result;
}

bool ToolBar::addButton(int iBitmap, int idCommand, BYTE state, BYTE style, UINT dwData, int iString)
{
  TBBUTTON tbb;

  tbb.iBitmap = iBitmap;
  tbb.idCommand = idCommand;
  tbb.fsState = state;
  tbb.fsStyle = style;
  tbb.dwData = dwData;
  tbb.iString = iString;

  LRESULT result = SendMessage(m_hWndToolbar, TB_ADDBUTTONS, 1, reinterpret_cast<LPARAM>(&tbb));
  if (result) {
    SendMessage(m_hWndToolbar, TB_AUTOSIZE, 0, 0);
  }
  return !!result;
}

bool ToolBar::checkButton(int iButton, bool check)
{
  LRESULT result = SendMessage(m_hWndToolbar, TB_CHECKBUTTON, iButton, MAKELONG(check, 0));
  return !!result;
}

bool ToolBar::enableButton(int idButton, bool enable)
{
    LRESULT result = SendMessage(m_hWndToolbar, TB_ENABLEBUTTON, idButton, MAKELONG(enable, 0));
    return !!result;
}

bool ToolBar::pressButton(int iButton, bool press)
{
  LRESULT result = SendMessage(m_hWndToolbar, TB_PRESSBUTTON, iButton, MAKELONG(press, 0));
  return !!result;
    return false;
}

bool ToolBar::getButtonRect(int nIndex, LPRECT buttonRect)
{
  LRESULT result = SendMessage(m_hWndToolbar, TB_GETITEMRECT, nIndex, (LPARAM)buttonRect);
  return !!result;
}

bool ToolBar::setButtonSize(int width, int height)
{
  LRESULT result = SendMessage(m_hWndToolbar, TB_SETBUTTONSIZE, 0, MAKELONG(width, height));
  if (result) {
    SendMessage(m_hWndToolbar, TB_AUTOSIZE, 0, 0);
    return true;
  }
  return false;
}

void ToolBar::autoSize()
{
  LRESULT style = SendMessage(m_hWndToolbar, TB_GETSTYLE, 0, 0);
  if (style & CCS_NORESIZE) {
    RECT r, btnRect;
    GetClientRect(GetParent(m_hWndToolbar), &r);
    getButtonRect(0, &btnRect);
    int height = getButtonsHeight() + btnRect.top * 2 + 2;
    SetWindowPos(m_hWndToolbar, HWND_TOP, 0, 0, r.right - r.left, height, SWP_NOMOVE);
  } else {
    SendMessage(m_hWndToolbar, TB_AUTOSIZE, 0, 0);
  }
}

int ToolBar::getButtonsHeight()
{
  return HIWORD(SendMessage(m_hWndToolbar, TB_GETBUTTONSIZE, 0, 0));
}

int ToolBar::getButtonsWidth()
{
  return LOWORD(SendMessage(m_hWndToolbar, TB_GETBUTTONSIZE, 0, 0));
}

bool ToolBar::isVisible()
{
  LRESULT style = GetWindowLong(m_hWndToolbar, GWL_STYLE);
  return !!(style & WS_VISIBLE);
}

void ToolBar::hide()
{
  ShowWindow(m_hWndToolbar, SW_HIDE);
}

void ToolBar::show()
{
  ShowWindow(m_hWndToolbar, SW_SHOW);
}

int ToolBar::getTotalWidth()
{
  SIZE size;
  SendMessage(m_hWndToolbar, TB_GETMAXSIZE, 0, reinterpret_cast<LPARAM>(&size));
  return size.cx;
}

int ToolBar::getHeight()
{
  RECT r;
  GetWindowRect(m_hWndToolbar, &r);
  return r.bottom - r.top;

}

LRESULT ToolBar::getState(int idButton)
{
  LRESULT result = SendMessage(m_hWndToolbar, TB_GETSTATE, idButton, 0);
  return result;
}
