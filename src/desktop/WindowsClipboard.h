#pragma once
#include "utils/CommonHeader.h"
#include "gui/MessageWindow.h"
#include "ClipboardListener.h"
#include "log_writer/LogWriter.h"
#include "thread/GuiThread.h"

class WindowsClipboard: protected MessageWindow, GuiThread
{
public:
  WindowsClipboard(ClipboardListener* clipboardListener, LogWriter* log);
  virtual ~WindowsClipboard(void);

  // This function replaces clipboard content by the text
  virtual bool writeToClipBoard(const TCHAR* text);

  virtual void readFromClipBoard(StringStorage* clipDest) const;

protected:
  virtual bool wndProc(UINT message, WPARAM wParam, LPARAM lParam);

  virtual void execute();
  virtual void onTerminate();

  void convertToRfbFormat(const StringStorage* source, StringStorage* dest);
  void convertFromRfbFormat(const TCHAR* source, StringStorage* dest);

  HWND m_hwndNextViewer;

  ClipboardListener* m_clipboardListener;

  LogWriter* m_log;

  static const HINSTANCE m_hinst;
};

