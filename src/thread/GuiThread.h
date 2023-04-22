#pragma once
#include "Thread.h"

class GuiThread: public Thread
{
public:
  GuiThread();
  virtual ~GuiThread();
private:
  // Replacing the base function.
  static DWORD WINAPI threadProc(LPVOID pThread);

  virtual void initByDerived();

  // Desktop for current thread.
  HDESK m_hDesk;
};

