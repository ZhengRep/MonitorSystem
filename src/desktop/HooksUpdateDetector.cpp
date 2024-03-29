#include "HooksUpdateDetector.h"
#include "mnt_server_app/NamingDefs.h"
#include "win_system/Environment.h"

HooksUpdateDetector::HooksUpdateDetector(UpdateKeeper* updateKeeper,
  UpdateListener* updateListener, LogWriter* log)
  : UpdateDetector(updateKeeper, updateListener),
  m_updateTimer(updateListener),
  m_targetWin(0),
  m_hookInstaller(0),
  m_log(log)
{
  try {
    m_hookInstaller = new HookInstaller();
  } catch (Exception& e) {
    Thread::terminate();
    m_log->error(_T("Failed to load the hook library: %s"), e.getMessage());
  }
  HINSTANCE hinst = GetModuleHandle(0);
  m_targetWin = new MessageWindow(hinst,  HookDefinitions::HOOK_TARGET_WIN_CLASS_NAME);
}

HooksUpdateDetector::~HooksUpdateDetector()
{
  terminate();
  wait();

  if (m_hookInstaller != 0) {
    delete m_hookInstaller;
  }
  if (m_targetWin != 0) {
    delete m_targetWin;
  }
}

void HooksUpdateDetector::onTerminate()
{
  if (m_targetWin != 0) {
    PostMessage(m_targetWin->getHWND(), WM_QUIT, 0, 0);
  }
  m_initWaiter.notify();
}

void HooksUpdateDetector::start32Loader()
{
#ifdef _WIN64
  if (!isTerminating()) {
    StringStorage path, folder;
    Environment::getCurrentModuleFolderPath(&folder);
    path.format(_T("%s\\%s"), folder.getString(), HookDefinitions::HOOK_LOADER_NAME);
    m_hookLoader32.setFilename(path.getString());
    StringStorage hwndStr;
    hwndStr.format(_T("%I64u"), (DWORD64)m_targetWin->getHWND());
    m_hookLoader32.setArguments(hwndStr.getString());
    try {
      m_hookLoader32.start();
    } catch (Exception& e) {
      m_log->error(_T("Can't run the 32-bit hook loader: %s"), e.getMessage());
    }
  }
#endif
}

void HooksUpdateDetector::terminate32Loader()
{
  if (m_hookLoader32.getProcessHandle() != 0) {
    // Send broadcast message to close the 32 bit hook loader.
    broadcastMessage(HookDefinitions::LOADER_CLOSE_CODE);
  }
}

void HooksUpdateDetector::broadcastMessage(UINT message)
{
  HWND hwndFound = FindWindowEx(HWND_MESSAGE, 0, 0, 0);
  while (hwndFound) {
    PostMessage(hwndFound, message, 0, 0);
    hwndFound = GetNextWindow(hwndFound, GW_HWNDNEXT);
  }
}

void HooksUpdateDetector::execute()
{
  m_log->info(_T("Hooks update detector thread id = %d"), getThreadId());

  if (!isTerminating() && m_targetWin != 0) {
    m_targetWin->createWindow();
    m_log->info(_T("Hooks target window has been created (hwnd = %d)"),
      m_targetWin->getHWND());
  }

  bool hookInstalled = false;
  while (!isTerminating() && !hookInstalled) {
    try {
      m_hookInstaller->install(m_targetWin->getHWND());
      hookInstalled = true;
    } catch (Exception& e) {
      m_log->error(_T("Hooks installing failed, wait for the next trying: %s"),
        e.getMessage());
      m_initWaiter.waitForEvent(5000);
      try {
        m_hookInstaller->uninstall();
      } catch (Exception& e) {
        m_log->error(_T("Hooks uninstalling failed: %s"),
          e.getMessage());
      }
    }
  }

  start32Loader();

  if (!isTerminating()) {
    m_log->info(_T("Hooks update detector has been successfully initialized"));
  }

  MSG msg;
  while (!isTerminating()) {
    if (PeekMessage(&msg, m_targetWin->getHWND(), 0, 0, PM_REMOVE) != 0) {
      if (msg.message == HookDefinitions::SPEC_IPC_CODE) {
        Rect rect((INT16)(msg.wParam >> 16), (INT16)(msg.wParam & 0xffff),
          (INT16)(msg.lParam >> 16), (INT16)(msg.lParam & 0xffff));
        if (!rect.isEmpty() && rect.isValid()) {
          m_updateKeeper->addChangedRect(&rect);
          m_updateTimer.sear();
        }
      } else {
        DispatchMessage(&msg);
      }
    } else {
      if (WaitMessage() == 0) {
        m_log->error(_T("Hooks update detector has failed"));
        Thread::terminate();
      }
    }
  }

  try {
    if (m_hookInstaller != 0) {
      m_hookInstaller->uninstall();
    }
    terminate32Loader();
  } catch (Exception& e) {
    m_log->error(_T("%s"), e.getMessage());
  }
  m_log->info(_T("Hooks update detector has been terminated."));
}
