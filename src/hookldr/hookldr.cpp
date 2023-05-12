#include "utils/CommonHeader.h"
#include "utils/StringParser.h"
#include "HookLoaderApplication.h"
#include "mnt_server_app/NamingDefs.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
  try {
    HookLoaderApplication application(hInstance, WindowNames::WINDOW_CLASS_NAME, lpCmdLine);
    return application.run();
  } catch (...) {
    return 1;
  }
}