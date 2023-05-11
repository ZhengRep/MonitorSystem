#include "NamingDefs.h"

const TCHAR NamingDefs::MIRROR_DRIVER_MESSAGE_WINDOW_CLASS_NAME[] = _T("MIRROR_DRIVER_MESSAGE_WINDOW_CLASS_NAME");

const TCHAR ProductNames::PRODUCT_NAME[] = _T("MNT");
const TCHAR ProductNames::SERVER_PRODUCT_NAME[] = _T("MNT Server");

const TCHAR RegistryPaths::SERVER_PATH[] = _T("Software\\MNT\\Server");
const TCHAR RegistryPaths::SERVER_REVERSE_CONN_HISTORY_PATH[] = _T("Software\\MNT\\Control\\ReverseConnectionHistory");
const TCHAR RegistryPaths::DISPATCHER_CONN_HISTORY_PATH[] = _T("Software\\MNT\\Control\\DispatcherConnectionHistory");

const TCHAR WindowNames::WINDOW_CLASS_NAME[] = _T("MNTWindowsApplicationClass");

const TCHAR LogNames::SERVER_LOG_FILE_STUB_NAME[] = _T("MNTServer");
const TCHAR LogNames::VIEWER_LOG_FILE_STUB_NAME[] = _T("MNTViewer");
const TCHAR LogNames::LOG_DIR_NAME[] = _T("MNT");
const TCHAR LogNames::LOG_PIPE_PUBLIC_NAME[] = _T("MNT_log_pipe_public_name");
const TCHAR LogNames::WIN_EVENT_PROVIDER_NAME[] = _T("MNTServer");

const TCHAR ClipboardNames::CLIPBOARD_WIN_CLASS_NAME[] = _T("MNTServer.Clipboard.MessageWindow");


const TCHAR ServiceNames::VNCCONTROL_START_REGISTRY_ENTRY_NAME[] = _T("MNTControl");
const TCHAR ServiceNames::SERVICE_NAME[] = _T("MNTServer");
const TCHAR ServiceNames::SERVICE_NAME_TO_DISPLAY[] = _T("MNT Server");

const TCHAR ServerApplicationNames::SERVER_INSTANCE_MUTEX_NAME[] = _T("MNTServerApplication");
const TCHAR ServerApplicationNames::CONTROL_APP_INSTANCE_MUTEX_NAME[] = _T("MNTServerServiceControlSlave");
const TCHAR ServerApplicationNames::FOR_SERVICE_CONTROL_APP_PIPE_NAME[] = _T("MNT_Service_Control");
const TCHAR ServerApplicationNames::FOR_APP_CONTROL_APP_SERVICE_PIPE_NAME[] = _T("MNT_Application_Control");

const TCHAR HookDefinitions::HOOK_LOADER_WINDOW_CLASS[] = _T("MNTHookLoaderWindowClass");
const TCHAR HookDefinitions::HOOK_TARGET_WIN_CLASS_NAME[] = _T("HookTargetWinClassName");
const TCHAR HookDefinitions::HOOK_LOADER_NAME[] = _T("hookldr.exe");
const UINT HookDefinitions::LOADER_CLOSE_CODE = RegisterWindowMessage(_T("MNT.HOOK.LOADER.CLOSE.CODE"));
const UINT HookDefinitions::SPEC_IPC_CODE = RegisterWindowMessage(_T("MNT.HOOK.MESSAGE.CODE"));

const TCHAR DefaultNames::DEFAULT_COMPUTER_NAME[] = _T("MNT Server");

