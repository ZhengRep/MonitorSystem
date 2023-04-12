#include "NamingDefs.h"

const TCHAR NamingDefs::MIRROR_DRIVER_MESSAGE_WINDOW_CLASS_NAME[] = _T("MIRROR_DRIVER_MESSAGE_WINDOW_CLASS_NAME");

const TCHAR ProductNames::PRODUCT_NAME[] = _T("VNC");
const TCHAR ProductNames::SERVER_PRODUCT_NAME[] = _T("VNC Server");

const TCHAR RegistryPaths::SERVER_PATH[] = _T("Software\\VNC\\Server");
const TCHAR RegistryPaths::SERVER_REVERSE_CONN_HISTORY_PATH[] = _T("Software\\VNC\\Control\\ReverseConnectionHistory");
const TCHAR RegistryPaths::DISPATCHER_CONN_HISTORY_PATH[] = _T("Software\\VNC\\Control\\DispatcherConnectionHistory");

const TCHAR WindowNames::WINDOW_CLASS_NAME[] = _T("VncWindowsApplicationClass");

const TCHAR LogNames::SERVER_LOG_FILE_STUB_NAME[] = _T("VncServer");
const TCHAR LogNames::VIEWER_LOG_FILE_STUB_NAME[] = _T("VncViewer");
const TCHAR LogNames::LOG_DIR_NAME[] = _T("VNC");
const TCHAR LogNames::LOG_PIPE_PUBLIC_NAME[] = _T("VNC_log_pipe_public_name");
const TCHAR LogNames::WIN_EVENT_PROVIDER_NAME[] = _T("VncServer");

const TCHAR ClipboardNames::CLIPBOARD_WIN_CLASS_NAME[] = _T("VncServer.Clipboard.MessageWindow");


const TCHAR ServiceNames::VNCCONTROL_START_REGISTRY_ENTRY_NAME[] = _T("VncControl");
const TCHAR ServiceNames::SERVICE_NAME[] = _T("VncServer");
const TCHAR ServiceNames::SERVICE_NAME_TO_DISPLAY[] = _T("VNC Server");

const TCHAR ServerApplicationNames::SERVER_INSTANCE_MUTEX_NAME[] = _T("VncServerApplication");
const TCHAR ServerApplicationNames::CONTROL_APP_INSTANCE_MUTEX_NAME[] = _T("VncServerServiceControlSlave");
const TCHAR ServerApplicationNames::FOR_SERVICE_CONTROL_APP_PIPE_NAME[] = _T("Vnc_Service_Control");
const TCHAR ServerApplicationNames::FOR_APP_CONTROL_APP_SERVICE_PIPE_NAME[] = _T("Vnc_Application_Control");

const TCHAR HookDefinitions::HOOK_LOADER_WINDOW_CLASS[] = _T("VncHookLoaderWindowClass");
const TCHAR HookDefinitions::HOOK_TARGET_WIN_CLASS_NAME[] = _T("HookTargetWinClassName");
const TCHAR HookDefinitions::HOOK_LOADER_NAME[] = _T("hookldr.exe");
const UINT HookDefinitions::LOADER_CLOSE_CODE = RegisterWindowMessage(_T("VNC.HOOK.LOADER.CLOSE.CODE"));
const UINT HookDefinitions::SPEC_IPC_CODE = RegisterWindowMessage(_T("VNC.HOOK.MESSAGE.CODE"));

const TCHAR DefaultNames::DEFAULT_COMPUTER_NAME[] = _T("VNC Server");


const char HttpStrings::HTTP_INDEX_PAGE_FORMAT[] =
"<HTML>\n"
"  <HEAD><TITLE>Vnc desktop [%.256s]</TITLE></HEAD>\n"
"  <BODY>\n"
"    <APPLET ARCHIVE=\"vnc-jviewer.jar\""
" CODE=\"com.glavsoft.viewer.Viewer\" WIDTH=1 HEIGHT=1>\n"
"      <PARAM NAME=\"PORT\" VALUE=\"%d\">\n"
"      <PARAM NAME=\"OpenNewWindow\" VALUE=\"YES\">\n"
"%.1024s"
"    </APPLET><BR>\n"
"    <A HREF=\"http://www.vnc.com/\">www.vnc.com</A>\n"
"  </BODY>\n"
"</HTML>\n";

const char HttpStrings::HTTP_MSG_BADPARAMS[] =
"<HTML>\n"
"  <HEAD><TITLE>vnc desktop</TITLE></HEAD>\n"
"  <BODY>\n"
"    <H1>Bad Parameters</H1>\n"
"    The sequence of applet parameters specified within the URL is invalid.\n"
"  </BODY>\n"
"</HTML>\n";
