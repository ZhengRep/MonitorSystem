
#include "Utils/CommonHeader.h"
#include "vnc_server_app/CrashHook.h"
#include "vnc_server_app/WinEventLogWriter.h"
#include "vnc_server_app/MntServerApplication.h"
#include "vnc_server_app/NamingDefs.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
  LogWriter preLog(NULL);
  WinEventLogWriter winEventLogWriter(&preLog);
  CrashHook crashHook(&winEventLogWriter);


  crashHook.setGuiEnabled();
  MntServerApplication tvnServer(hInstance,
    WindowNames::WINDOW_CLASS_NAME,
    lpCmdLine, &winEventLogWriter);

  return tvnServer.run();
  return 0;
}
