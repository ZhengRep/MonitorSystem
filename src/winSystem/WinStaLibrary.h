#pragma once
#include "Utils/CommonHeader.h"
#include "DynamicLibrary.h"

typedef BOOL(WINAPI* pWinStationConnectW)(HANDLE, ULONG, ULONG, PCWSTR, ULONG);

class WinStaLibrary
{
public:
  WinStaLibrary();
  virtual ~WinStaLibrary();

  BOOL WinStationConnectW(HANDLE server, ULONG connectSessionId,ULONG activeSessionId, PCWSTR password, ULONG unknown);

protected:
  DynamicLibrary* m_winStaDll;

  pWinStationConnectW m_winStationConnectW;
};

