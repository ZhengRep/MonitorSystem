#include "WsaStartup.h"
#include "utils/Exception.h"

WsaStartup::WsaStartup(BYTE minorVer, BYTE majorVer)
{
  WSAData wsaData;
  if (WSAStartup(MAKEWORD(majorVer, minorVer), &wsaData) != 0) {
    throw Exception(_T("Failed to initialize WsaStartup."));
  }
}

WsaStartup::~WsaStartup()
{
  WSACleanup();
}
