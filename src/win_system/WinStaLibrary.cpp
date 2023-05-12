#include "WinStaLibrary.h"

WinStaLibrary::WinStaLibrary()
  :m_winStaDll(NULL), m_winStationConnectW(NULL)
{
  try {
    m_winStaDll = new DynamicLibrary(_T("winsta.dll"));

    m_winStationConnectW = (pWinStationConnectW)m_winStaDll->getProcAddress("WinStationConnectW");
  } catch (...) {
    m_winStaDll = NULL;
  }
}

WinStaLibrary::~WinStaLibrary()
{
  if (m_winStaDll != NULL) {
    delete m_winStaDll;
  }
}

BOOL WinStaLibrary::WinStationConnectW(HANDLE server, ULONG connectSessionId, ULONG activeSessionId, PCWSTR password, ULONG unknown)
{
  if (m_winStaDll == 0 || m_winStationConnectW == 0) {
    return FALSE;
  }
  return m_winStationConnectW(server, connectSessionId, activeSessionId, password, unknown);
}
