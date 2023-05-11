#include "Utils/CommonHeader.h"
#include "Utils/StringParser.h"
#include "HookLoaderApplication.h"
#include "vnc_server_app/NamingDefs.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
  try {
    HookLoaderApplication application(hInstance, WindowNames::WINDOW_CLASS_NAME, lpCmdLine);
    return application.run();
  } catch (...) {
    return 1;
  }
}