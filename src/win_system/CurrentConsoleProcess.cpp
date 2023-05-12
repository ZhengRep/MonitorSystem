#include "CurrentConsoleProcess.h"
#include "WTS.h"

CurrentConsoleProcess::CurrentConsoleProcess(LogWriter* log, bool connectRdpSession, const TCHAR* path, const TCHAR* args)
  :Process(path, args),
  m_log(log),
  m_connectRdpSession(connectRdpSession)
{
}

CurrentConsoleProcess::~CurrentConsoleProcess()
{
}

void CurrentConsoleProcess::start() throw(SystemException)
{
  cleanup();
  DWORD rdpSession = 0;
  DWORD sessionId;

  if (m_connectRdpSession) {
    rdpSession = WTS::getRdpSessionId(m_log);
  }
  if (rdpSession) {
    sessionId = rdpSession;
    m_log->info(_T("Connect as RDP user at %d session"), sessionId);
    m_log->info(_T("Try to start \"%s %s\" process"), m_path.getString(), m_args.getString());
  } else {
    sessionId = WTS::getActiveConsoleSessionId(m_log); m_log->info(_T("Connect as current user at %d session"), sessionId);
    m_log->info(_T("Try to start \"%s %s\" process"), m_path.getString(), m_args.getString());
  }

  DWORD uiAccess = 1;
  PROCESS_INFORMATION pi;
  STARTUPINFO sti;
  getStartupInfo(&sti);

  m_log->debug(_T("sti: cb = %d, hStdError = %p, hStdInput = %p, hStdOutput = %p, dwFlags = %u"),
    (unsigned int)sti.cb, (void*)sti.hStdError, (void*)sti.hStdInput, (void*)sti.hStdOutput, (unsigned int)sti.dwFlags);

  HANDLE procHandle = GetCurrentProcess();
  HANDLE token, userToken;

  try {
    m_log->debug(_T("Try OpenProcessToken(%p, , )"), (void*)procHandle);

    if (OpenProcessToken(procHandle, TOKEN_DUPLICATE, &token) == 0) {
      throw SystemException();
    }

    m_log->debug(_T("Try DuplicateTokenEx(%p, , , , , )"), (void*)token);
    if (DuplicateTokenEx(token, MAXIMUM_ALLOWED, 0, SecurityImpersonation, TokenPrimary, &userToken) == 0) {
      throw SystemException();
    }

    m_log->debug(_T("Try SetTokenInformation(%p, , , )"), (void*)userToken);
    if (SetTokenInformation(userToken, (TOKEN_INFORMATION_CLASS)TokenSessionId, &sessionId, sizeof(sessionId)) == 0) {
      throw SystemException();
    }


    // Fix Windows 8/8.1 UIAccess restrictions (Alt-Tab) for server as service
     // http://stackoverflow.com/questions/13972165/pressing-winx-alt-tab-programatically
     // For application we need to set /uiAccess='true' in linker manifest, sign binary 
     // and run from "Program Files/"
    m_log->debug(_T("Try SetTokenInformation(%p, , , ) with UIAccess=1"), (void*)userToken);
    if (SetTokenInformation(userToken, (TOKEN_INFORMATION_CLASS)TokenUIAccess, &uiAccess, sizeof(uiAccess)) == 0) {
      m_log->info(_T("Can't set UIAccess=1, ignore it"), (void*)userToken);
    }

    StringStorage commandLine = getCommandLineString();

    m_log->debug(_T("Try CreateProcessAsUser(%p, 0, %s, 0, 0, %d, NORMAL_PRIORITY_CLASS, 0, 0, sti, pi)"),
      (void*)userToken, commandLine.getString(), (int)m_handlesIsInherited);

    if (CreateProcessAsUser(userToken, 0, (LPTSTR)commandLine.getString(), 0, 0, m_handlesIsInherited, NORMAL_PRIORITY_CLASS, 
      0, 0, &sti, &pi) == 0) {
      throw SystemException();
    }
    m_log->info(_T("Created \"%s\" process at %d windows session"), commandLine.getString(), sessionId);
  } catch (SystemException& sysEx) {
    m_log->error(_T("Failed to start process with %d error"), sysEx.getErrorCode());
    throw;
  }

  CloseHandle(userToken);
  CloseHandle(token);

  m_hThread = pi.hThread;
  m_hProcess = pi.hProcess;
}
