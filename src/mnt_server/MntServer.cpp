#include "Utils/CommonHeader.h"
#include "Utils/winhdr.h"
#include "Utils/CommandLine.h"
#include "winSystem/WinCommandLineArgs.h"
#include "vnc_server_app/WinEventLogWriter.h"
#include "vnc_server_app/CrashHook.h"
#include "log_writer/LogWriter.h"
#include "Utils/ResourceLoader.h"
#include "mnt_control_app/ControlCommandLine.h"
#include "vnc_server_app/MntService.h"
#include "vnc_server_app/DesktopServerCommandLine.h"
#include "vnc_server_app/QueryConnectionCommandLine.h"
#include "vnc_server_app/AdditionalActionApplication.h"
#include "vnc_server_app/ServiceControlCommandLine.h"
#include "mnt_control_app/ControlApplication.h"
#include "vnc_server_app/NamingDefs.h"
#include "vnc_server_app/DesktopServerApplication.h"
#include "vnc_server_app/QueryConnectionApplication.h"
#include "vnc_server_app/ServiceControlApplication.h"
#include "vnc_server_app/MntServerApplication.h"
#include "mnt_server/resource.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
  LogWriter preLog(0);

  WinEventLogWriter winEventLogWriter(&preLog);
  CrashHook crashHook(&winEventLogWriter);

  ResourceLoader resourceLoaderSingleton(hInstance);

  /*CommandLineFormat format[] = {
    { MntService::SERVICE_COMMAND_LINE_KEY, NO_ARG },*/

   /* { ControlCommandLine::CONFIG_APPLICATION, NO_ARG },
    { ControlCommandLine::CONFIG_SERVICE, NO_ARG },
    { ControlCommandLine::SET_CONTROL_PASSWORD, NO_ARG },
    { ControlCommandLine::SET_PRIMARY_MNT_PASSWORD, NO_ARG },
    { ControlCommandLine::CHECK_SERVICE_PASSWORDS, NO_ARG },
    { ControlCommandLine::CONTROL_SERVICE, NO_ARG },
    { ControlCommandLine::CONTROL_APPLICATION, NO_ARG },

    { DesktopServerCommandLine::DESKTOP_SERVER_KEY, NO_ARG },
    { QueryConnectionCommandLine::QUERY_CONNECTION, NO_ARG },

    { AdditionalActionApplication::LOCK_WORKSTATION_KEY, NO_ARG },
    { AdditionalActionApplication::LOGOUT_KEY, NO_ARG },

    { ServiceControlCommandLine::INSTALL_SERVICE },
    { ServiceControlCommandLine::REMOVE_SERVICE },
    { ServiceControlCommandLine::REINSTALL_SERVICE },
    { ServiceControlCommandLine::START_SERVICE },
    { ServiceControlCommandLine::STOP_SERVICE }*/
  //};

  //CommandLine parser;
  //StringStorage firstKey(_T(""));


  //try {
  //  WinCommandLineArgs args(lpCmdLine);
  //  parser.parse(format, sizeof(format) / sizeof(CommandLineFormat), &args);
  //} catch (...) {
  //}
  //parser.getOption(0, &firstKey);

  //if (firstKey.isEqualTo(MntService::SERVICE_COMMAND_LINE_KEY)) {
  //  MntService mntService(&winEventLogWriter, &winEventLogWriter);
  //  try {
  //    crashHook.setHklmRoot();
  //    mntService.run();
  //  } catch (Exception&) {
  //    return 1;
  //  }
  //  return 0;
  //} else if (firstKey.isEqualTo(ControlCommandLine::CONFIG_APPLICATION) ||
  //  firstKey.isEqualTo(ControlCommandLine::CONFIG_SERVICE) ||
  //  firstKey.isEqualTo(ControlCommandLine::SET_CONTROL_PASSWORD) ||
  //  firstKey.isEqualTo(ControlCommandLine::SET_PRIMARY_MNT_PASSWORD) ||
  //  firstKey.isEqualTo(ControlCommandLine::CONTROL_SERVICE) ||
  //  firstKey.isEqualTo(ControlCommandLine::CONTROL_APPLICATION) ||
  //  firstKey.isEqualTo(ControlCommandLine::CHECK_SERVICE_PASSWORDS)) {
  //  crashHook.setGuiEnabled();
  //  try {
  //    ControlApplication mntControl(hInstance, WindowNames::WINDOW_CLASS_NAME, lpCmdLine);
  //    return mntControl.run();
  //  } catch (Exception& fatalException) {
  //    MessageBox(0, fatalException.getMessage(),
  //      StringTable::getString(IDS_MBC_MNTCONTROL),
  //      MB_OK | MB_ICONERROR);
  //    return 1;
  //  }
  //} else if (firstKey.isEqualTo(AdditionalActionApplication::LOCK_WORKSTATION_KEY) ||
  //  firstKey.isEqualTo(AdditionalActionApplication::LOGOUT_KEY)) {
  //  crashHook.setGuiEnabled();
  //  try {
  //    AdditionalActionApplication actionApp(hInstance,
  //      WindowNames::WINDOW_CLASS_NAME,
  //      lpCmdLine);
  //    return actionApp.run();
  //  } catch (SystemException& ex) {
  //    return ex.getErrorCode();
  //  }
  //} else if (firstKey.isEqualTo(DesktopServerCommandLine::DESKTOP_SERVER_KEY)) {
  //  try {
  //    crashHook.setHklmRoot();
  //    WinCommandLineArgs args(lpCmdLine);
  //    DesktopServerApplication desktopServerApp(hInstance, WindowNames::WINDOW_CLASS_NAME, &args);

  //    int retCode = desktopServerApp.run();
  //    return retCode;
  //  } catch (...) {
  //    return 1;
  //  }
  //} else if (firstKey.isEqualTo(QueryConnectionCommandLine::QUERY_CONNECTION)) {
  //  crashHook.setGuiEnabled();
  //  try {
  //    QueryConnectionApplication app(hInstance, WindowNames::WINDOW_CLASS_NAME, lpCmdLine);
  //    return app.run();
  //  } catch (...) {
  //    return 1;
  //  }
  //} else if (firstKey.isEqualTo(ServiceControlCommandLine::INSTALL_SERVICE) ||
  //  firstKey.isEqualTo(ServiceControlCommandLine::REMOVE_SERVICE) ||
  //  firstKey.isEqualTo(ServiceControlCommandLine::REINSTALL_SERVICE) ||
  //  firstKey.isEqualTo(ServiceControlCommandLine::START_SERVICE) ||
  //  firstKey.isEqualTo(ServiceControlCommandLine::STOP_SERVICE)) {
  //  crashHook.setGuiEnabled();
  //  ServiceControlApplication tvnsc(hInstance, WindowNames::WINDOW_CLASS_NAME, lpCmdLine);
  //  return tvnsc.run();
  //}

  // No additional applications, run mnt server as single application.
  crashHook.setGuiEnabled();
  MntServerApplication mntServer(hInstance, WindowNames::WINDOW_CLASS_NAME, lpCmdLine, &winEventLogWriter);

  return mntServer.run();
}