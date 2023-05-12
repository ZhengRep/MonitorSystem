#include "CrashHook.h"
#include "win_system/RegistryKey.h"
#include "win_system/DynamicLibrary.h"
#include "win_system/Environment.h"
#include "mnt_server_app/NamingDefs.h"

typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)(__in  HANDLE hProcess,
  __in  DWORD ProcessId,
  __in  HANDLE hFile,
  __in  MINIDUMP_TYPE DumpType,
  __in  PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
  __in  PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
  __in  PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

bool CrashHook::m_guiEnabled = false;
HKEY CrashHook::m_rootHkey = HKEY_CURRENT_USER;
LocalMutex CrashHook::m_guiEnabledMutex;
ApplicationCrashEvents* CrashHook::m_notifier = 0;

CrashHook::CrashHook(ApplicationCrashEvents* notifier)
{
  m_notifier = notifier;
  ::SetUnhandledExceptionFilter(topLevelExceptionFilter);
}

CrashHook::~CrashHook()
{
}

LONG WINAPI CrashHook::topLevelExceptionFilter(_EXCEPTION_POINTERS* pExceptionInfo)
{
  bool guiEnabled;
  {
    AutoLock al(&m_guiEnabledMutex);
    guiEnabled = m_guiEnabled;
  }

  LONG retValue = EXCEPTION_CONTINUE_SEARCH;

  DynamicLibrary dbgLib;
  MINIDUMPWRITEDUMP miniDumpWriteDump = 0;
  try {
    // Try load the library from this exe mofule folder
    StringStorage libName, moduleFolder;
    Environment::getCurrentModuleFolderPath(&moduleFolder);
    libName.format(_T("%s\\DbgHelp.dll"), moduleFolder.getString());

    dbgLib.init(libName.getString());
    miniDumpWriteDump = (MINIDUMPWRITEDUMP)dbgLib.getProcAddress("MiniDumpWriteDump");
  } catch (...) {
    try {
      // Try load the library by default path
      dbgLib.init(_T("DbgHelp.dll"));
      miniDumpWriteDump = (MINIDUMPWRITEDUMP)dbgLib.getProcAddress("MiniDumpWriteDump");
    } catch (...) {
      return retValue;
    }
  }
  if (miniDumpWriteDump == 0) {
    return retValue;
  }

  StringStorage specFolder, dumpPath;
  if (!Environment::getSpecialFolderPath(
    Environment::APPLICATION_DATA_SPECIAL_FOLDER, &specFolder)) {
    return retValue;
  }
  dumpPath.format(_T("%s\\%s\\crash.dmp"), specFolder.getString(),
    ProductNames::PRODUCT_NAME);

  if (guiEnabled && MessageBox(0,
    _T("Apllication crashing. Do you")
    _T(" want save debug information?"),
    ProductNames::PRODUCT_NAME,
    MB_YESNO)
    != IDYES)
  {
    return retValue;
  }
  HANDLE hFile = 0;
  try {
    // Store path to the registry.
    try {
      HKEY root;
      {
        AutoLock al(&m_guiEnabledMutex);
        root = m_rootHkey;
      }
      RegistryKey regKey(root, RegistryPaths::SERVER_PATH);
      regKey.setValueAsString(_T("CrashDumpPath"), dumpPath.getString());
    } catch (Exception&) {
    }

    hFile = ::CreateFile(dumpPath.getString(),
      GENERIC_WRITE,
      FILE_SHARE_WRITE,
      0,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      0);
    if (hFile == INVALID_HANDLE_VALUE) {
      throw Exception(_T("Cannot create file to save a debug information"));
    }

    _MINIDUMP_EXCEPTION_INFORMATION exInfo;

    exInfo.ThreadId = ::GetCurrentThreadId();
    exInfo.ExceptionPointers = pExceptionInfo;
    exInfo.ClientPointers = 0;

    BOOL result = miniDumpWriteDump(GetCurrentProcess(),
      GetCurrentProcessId(),
      hFile,
      MiniDumpNormal,
      &exInfo,
      0,
      0);
    if (result == 0) {
      throw Exception(_T("Cannot create the crash dump file"));
    }
    if (guiEnabled) {
      StringStorage succMess;
      succMess.format(_T("The debug information has been successfully")
        _T(" saved to the %s file"), dumpPath.getString());
      MessageBox(0, succMess.getString(), ProductNames::PRODUCT_NAME, MB_OK);
    }
    m_notifier->onCrash(&dumpPath);

    retValue = EXCEPTION_EXECUTE_HANDLER;
  } catch (Exception& e) {
    if (guiEnabled) {
      MessageBox(NULL, e.getMessage(), ProductNames::PRODUCT_NAME, MB_OK);
    }
  }
  if (hFile != 0) {
    CloseHandle(hFile);
  }

  return retValue;
}

void CrashHook::setGuiEnabled()
{
  AutoLock al(&m_guiEnabledMutex);
  m_guiEnabled = true;
}

void CrashHook::setHklmRoot()
{
  AutoLock al(&m_guiEnabledMutex);
  m_rootHkey = HKEY_LOCAL_MACHINE;
}
