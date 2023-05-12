#include "DesktopServerWatcher.h"
#include "win_system/Environment.h"
#include "utils/Exception.h"
#include "server_config_lib/Configurator.h"
#include "win_system/CurrentConsoleProcess.h"
#include "win_system/AnonymousPipeFactory.h"
#include "win_system/EmulatedAnonymousPipeFactory.h"
#include "win_system/WTS.h"
#include "win_system/WinStaLibrary.h"
#include "win_system/ShareMemory.h"
#include "win_system/WinHandles.h"
#include "win_system/ShareMemory.h"
#include <time.h>

DesktopServerWatcher::DesktopServerWatcher(ReconnectionListener* recListener, LogWriter* log)
  : m_recListener(recListener),
    m_process(0),
    m_log(log)
{
  // Desktop server folder.
  StringStorage currentModulePath;
  Environment::getCurrentModulePath(&currentModulePath);

  // Path to desktop server application.
  StringStorage path;
  // FIXME: To think: is quotes needed?
  path.format(_T("\"%s\""), currentModulePath.getString());

  try {
    bool connectRdpSession = Configurator::getInstance()->getServerConfig()->getConnectToRdpFlag();
    m_process = new CurrentConsoleProcess(m_log, connectRdpSession, path.getString());
  } catch (...) {
    if (m_process) delete m_process;
    throw;
  }
}

DesktopServerWatcher::~DesktopServerWatcher()
{
  terminate();
  wait();
  delete m_process;
}

void DesktopServerWatcher::execute()
{
  AnonymousPipeFactory pipeFactory(512 * 1024, m_log);

  AnonymousPipe* ownSidePipeChanTo, * otherSidePipeChanTo, * ownSidePipeChanFrom, * otherSidePipeChanFrom;

  ownSidePipeChanTo = otherSidePipeChanTo = ownSidePipeChanFrom = otherSidePipeChanFrom = 0;

  while (!isTerminating()) {
    try {
      StringStorage shMemName(_T("Global\\"));
      srand((unsigned)time(0));
      for (int i = 0; i < 20; i++) {
        shMemName.appendChar('a' + rand() % ('z' - 'a'));
      }
      SharedMemory sharedMemory(shMemName.getString(), 72);
      UINT64* mem = (UINT64*)sharedMemory.getMemPointer();

      // Sets memory ready flag to false.
      mem[0] = 0;


      pipeFactory.generatePipes(&ownSidePipeChanTo, false, &otherSidePipeChanTo, false);
      pipeFactory.generatePipes(&ownSidePipeChanFrom, false, &otherSidePipeChanFrom, false);

      //server log directory.
      StringStorage logDir;
      Configurator::getInstance()->getServerConfig()->getLogFileDir(&logDir);

      // Arguments that must be passed to desktop server application.
      StringStorage args;
      args.format(_T("-desktopserver -logdir \"%s\" -loglevel %d -shmemname %s"), logDir.getString(),
        Configurator::getInstance()->getServerConfig()->getLogLevel(), shMemName.getString());

      m_process->setArguments(args.getString());
      start();

      // Prepare other side pipe handles for other side
      m_log->debug(_T("DesktopServerWatcher::execute(): assigning handles"));
      otherSidePipeChanTo->assignHandlesFor(m_process->getProcessHandle(), false);
      otherSidePipeChanFrom->assignHandlesFor(m_process->getProcessHandle(), false);

      // Transfer other side handles by the memory channel
      mem[1] = (UINT64)otherSidePipeChanTo->getWriteHandle();
      mem[2] = (UINT64)otherSidePipeChanTo->getReadHandle();
      mem[3] = (UINT64)otherSidePipeChanTo->getMaxPortionSize();
      mem[4] = (UINT64)otherSidePipeChanFrom->getWriteHandle();
      mem[5] = (UINT64)otherSidePipeChanFrom->getReadHandle();
      mem[6] = (UINT64)otherSidePipeChanFrom->getMaxPortionSize();

      // Sets memory ready flag to true.
      mem[0] = 1;

      // Destroying other side objects
      delete otherSidePipeChanTo;
      m_log->debug(_T("DesktopServerWatcher::execute(): Destroyed otherSidePipeChanTo"));
      otherSidePipeChanTo = 0;
      delete otherSidePipeChanFrom;
      m_log->debug(_T("DesktopServerWatcher::execute(): Destroyed otherSidePipeChanFrom"));
      otherSidePipeChanFrom = 0;

      m_log->debug(_T("DesktopServerWatcher::execute(): Try to call onReconnect()"));
      m_recListener->onReconnect(ownSidePipeChanTo, ownSidePipeChanFrom);

      m_process->waitForExit();

    } catch (Exception& e) {
      // A potentional memory leak. 
      // A potential crash. The channels can be used (see onReconnect()) after these destroyings.
      if (ownSidePipeChanTo) delete ownSidePipeChanTo;
      if (otherSidePipeChanTo) delete otherSidePipeChanTo;
      if (ownSidePipeChanFrom) delete ownSidePipeChanFrom;
      if (otherSidePipeChanFrom) delete otherSidePipeChanFrom;
      m_log->error(_T("DesktopServerWatcher has failed with error: %s"), e.getMessage());
      Sleep(1000);
    }
  }
}

void DesktopServerWatcher::onTerminate()
{
  m_process->stopWait();
}

void DesktopServerWatcher::start()
{
  int pipeNotConnectedErrorCount = 0;

  for (int i = 0; i < 5; i++) {
    try {
      m_process->start();
      return;
    } catch (SystemException& sysEx) {
      // It can be XP specific error.
      if (sysEx.getErrorCode() == 233 || sysEx.getErrorCode() == 87) {
        pipeNotConnectedErrorCount++;

        DWORD sessionId = WTS::getActiveConsoleSessionId(m_log);

        bool isXPFamily = Environment::isWinXP() || Environment::isWin2003Server();
        bool needXPTrick = (isXPFamily) && (sessionId > 0) && (pipeNotConnectedErrorCount >= 3);

        // Try start as current user with xp trick.
        if (needXPTrick) {
          doXPTrick();
          m_process->start();
          return;
        }
      } else {
        throw;
      }
    }
    Sleep(3000);
  } // for 
}

void DesktopServerWatcher::doXPTrick()
{
  m_log->info(_T("Trying to do WindowsXP trick to start process on separate session"));

  try {
    WinStaLibrary winSta;

    WCHAR password[1];
    memset(password, 0, sizeof(password));

    if (winSta.WinStationConnectW(NULL, 0, WTS::getActiveConsoleSessionId(m_log),
      password, 0) == FALSE) {
      throw SystemException(_T("Failed to call WinStationConnectW"));
    }

    // Get path to server binary.
    StringStorage pathToBinary;
    Environment::getCurrentModulePath(&pathToBinary);

    // Start current console process that will lock workstation (not using Xp Trick).
    CurrentConsoleProcess lockWorkstation(m_log, false, pathToBinary.getString(), _T("-lockworkstation"));
    lockWorkstation.start();
    lockWorkstation.waitForExit();

    // Check exit code (exit code is GetLastError() value in case of system error,
    // LockWorkstation() in child process failed, or 0 if workstation is locked).
    DWORD exitCode = lockWorkstation.getExitCode();

    if (exitCode != 0) {
      throw SystemException(exitCode);
    }
  } catch (SystemException& ex) {
    m_log->error(ex.getMessage());
    throw;
  }
}
