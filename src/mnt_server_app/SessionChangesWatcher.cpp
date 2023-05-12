#include "SessionChangesWatcher.h"
#include "win_system/WTS.h"

SessionChangesWatcher::SessionChangesWatcher(AnEventListener* extSessionChangesListener,
  LogWriter* log)
  : m_extSessionChangesListener(extSessionChangesListener),
  m_log(log)
{
  ProcessIdToSessionId(GetCurrentProcessId(), &m_baseSessionId);
  resume();
}

SessionChangesWatcher::~SessionChangesWatcher()
{
  terminate();
  wait();
}

void SessionChangesWatcher::execute()
{
  DWORD prevSession = m_baseSessionId;
  bool isRdp = WTS::SessionIsRdpSession(prevSession);
  StringStorage prevDeskName, currDeskName;
  DesktopSelector::getThreadDesktopName(&prevDeskName);

  while (!isTerminating()) {
    DWORD currSessionId = prevSession;
    if (!isRdp) {
      currSessionId = WTS::getActiveConsoleSessionId(m_log);
    }
    bool sessionChanged = prevSession != currSessionId;
    bool desktopInfoIsAvailable = DesktopSelector::getCurrentDesktopName(&currDeskName);
    bool desktopChanged = !currDeskName.isEqualTo(&prevDeskName);
    if (sessionChanged || desktopChanged || !desktopInfoIsAvailable) {
      m_log->debug(_T("Session or desktop has been changed.")
        _T(" The process session = %u, current session = %u")
        _T(" The process desktop = %s, current desktop = %s"),
        (unsigned int)prevSession, (unsigned int)currSessionId,
        prevDeskName.getString(), currDeskName.getString());
      prevSession = currSessionId;
      prevDeskName = currDeskName;
      m_extSessionChangesListener->onAnObjectEvent();
      terminate();
    } else {
      // FIXME: Use WindowsEvent instead of Sleep().
      Sleep(100);
    }
  }
}
