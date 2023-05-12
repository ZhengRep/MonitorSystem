#pragma once
#include "utils/CommonHeader.h"
#include "utils/Singleton.h"
#include "thread/LocalMutex.h"
#include "win_system/ApplicationCrashEvents.h"

class CrashHook: private Singleton<CrashHook>
{
public:
  CrashHook(ApplicationCrashEvents* notifier);
  virtual ~CrashHook();
  void setGuiEnabled();
  void setHklmRoot();
private:
  static LONG WINAPI topLevelExceptionFilter(_EXCEPTION_POINTERS* pExceptionInfo);

  static bool m_guiEnabled;
  static HKEY m_rootHkey;
  static LocalMutex m_guiEnabledMutex;

  static ApplicationCrashEvents* m_notifier;
};

