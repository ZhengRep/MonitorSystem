#pragma once

#include "Utils/CommonHeader.h"

class ToolBar
{
public:
  ToolBar();
  virtual ~ToolBar();

  static const int TB_Style_sep = 0;
  static const int TB_Style_gap = 1;

  void setViewAutoButtons(int iButton, int style);
  void loadToolBarfromRes(DWORD id);
  void setButtonsRange(DWORD id);
  void attachToolBar(HWND hwnd);
  bool create(int tbID, HWND parentHwnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT);
  LRESULT addBitmap(int nButtons, UINT bitmapID);
  LRESULT addSystemBitmap(UINT stdBitmapID);
  bool addNButton(int nButtons, LPTBBUTTON tbb);
  bool addButton(int iBitmap, int idCommand, BYTE state = TBSTATE_ENABLED, BYTE style = TBSTYLE_BUTTON, UINT dwData = 0, int iString = 0);
  bool checkButton(int iButton, bool check);
  bool enableButton(int idButton, bool enable);
  bool pressButton(int iButton, bool press);
  bool getButtonRect(int nIndex, LPRECT buttonRect);
  bool setButtonSize(int width, int height);
  void autoSize();
  int getButtonsHeight();
  int getButtonsWidth();
  bool isVisible();
  void hide();
  void show();
  int getTotalWidth();
  int getHeight();
  LRESULT getState(int idButton);

private:
  int m_initialStr;
  int m_numberTB;
  DWORD m_id;
  int m_width, m_height;
  HWND m_hWndToolbar;
  std::map<int, int> m_autoButtons;
};

