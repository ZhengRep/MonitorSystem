#pragma once
#include "Utils/CommonHeader.h"

class NamingDefs
{
public:
  static const TCHAR MIRROR_DRIVER_MESSAGE_WINDOW_CLASS_NAME[];
};

class ProductNames
{
public:
  static const TCHAR PRODUCT_NAME[];
  static const TCHAR SERVER_PRODUCT_NAME[];
};

class RegistryPaths
{
public:
  static const TCHAR SERVER_PATH[];
  static const TCHAR SERVER_REVERSE_CONN_HISTORY_PATH[];
  static const TCHAR DISPATCHER_CONN_HISTORY_PATH[];
};

class WindowNames
{
public:
  static const TCHAR WINDOW_CLASS_NAME[];
};

class LogNames
{
public:
  static const TCHAR SERVER_LOG_FILE_STUB_NAME[];
  static const TCHAR VIEWER_LOG_FILE_STUB_NAME[];
  static const TCHAR LOG_DIR_NAME[];
  static const TCHAR LOG_PIPE_PUBLIC_NAME[];
  static const TCHAR WIN_EVENT_PROVIDER_NAME[];
};

class ClipboardNames
{
public:
  static const TCHAR CLIPBOARD_WIN_CLASS_NAME[];
};

class ServiceNames
{
public:
  static const TCHAR MNTCONTROL_START_REGISTRY_ENTRY_NAME[];
  static const TCHAR SERVICE_NAME[];
  static const TCHAR SERVICE_NAME_TO_DISPLAY[];
};

class ServerApplicationNames
{
public:
  static const TCHAR SERVER_INSTANCE_MUTEX_NAME[];
  static const TCHAR CONTROL_APP_INSTANCE_MUTEX_NAME[];
  static const TCHAR FOR_SERVICE_CONTROL_APP_PIPE_NAME[];
  static const TCHAR FOR_APP_CONTROL_APP_SERVICE_PIPE_NAME[];
};

class HookDefinitions
{
public:
  static const TCHAR HOOK_LOADER_WINDOW_CLASS[];
  static const TCHAR HOOK_TARGET_WIN_CLASS_NAME[];
  static const TCHAR HOOK_LOADER_NAME[];
  static const UINT LOADER_CLOSE_CODE;
  static const UINT SPEC_IPC_CODE;
};

class DefaultNames
{
public:
  static const TCHAR DEFAULT_COMPUTER_NAME[];
};

class HttpStrings
{
public:
  static const char HTTP_INDEX_PAGE_FORMAT[];
  static const char HTTP_MSG_BADPARAMS[];
};
