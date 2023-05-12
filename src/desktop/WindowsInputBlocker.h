#pragma once
#include "utils/CommonHeader.h"
#include "thread/GuiThread.h"
#include "thread/LocalMutex.h"
#include "win_system/WindowsEvent.h"
#include "win_system/WinTimeMillis.h"
#include "InputBlocker.h"
#include "log_writer/LogWriter.h"

class WindowsInputBlocker: public InputBlocker, protected GuiThread
{
public:
  WindowsInputBlocker(LogWriter* log);
  virtual ~WindowsInputBlocker();
  virtual void setKeyboardBlocking(bool block);
  virtual void setMouseBlocking(bool block);
  virtual void setSoftKeyboardBlocking(bool block, unsigned int timeInterval);
  virtual void setSoftMouseBlocking(bool block, unsigned int timeInterval);

  virtual DateTime getLastInputTime() const;
  virtual void correctLastTime(DateTime newTime);

  virtual bool isRemoteInputAllowed();

protected:
  virtual void execute();
  virtual void onTerminate();

  bool setKeyboardFilterHook(bool block);
  bool setSoftKeyboardFilterHook(bool block);
  bool setMouseFilterHook(bool block);
  bool setSoftMouseFilterHook(bool block);

  static LRESULT CALLBACK lowLevelKeyboardFilterProc(int nCode, WPARAM wParam, LPARAM lParam);
  static LRESULT CALLBACK lowLevelSoftKeyboardFilterProc(int nCode, WPARAM wParam, LPARAM lParam);
  
  static LRESULT CALLBACK lowLevelMouseFilterProc(int nCode, WPARAM wParam, LPARAM lParam);
  static LRESULT CALLBACK lowLevelSoftMouseFilterProc(int nCode, WPARAM wParam, LPARAM lParam);

  static HHOOK m_hKeyboardHook;
  static HHOOK m_hSoftKeyboardHook;
  static HHOOK m_hMouseHook;
  static HHOOK m_hSoftMouseHook;

  static WindowsInputBlocker* m_instance;
  static LocalMutex m_instanceMutex;

  bool m_isKeyboardBlocking;
  bool m_isMouseBlocking;

  bool m_isSoftKeyboardBlocking;
  bool m_isSoftMouseBlocking;

  static unsigned int m_timeInterval;
  static DateTime m_lastInputTime;
  static LocalMutex m_lastInputTimeMutex;

  LogWriter* m_log;

};

