#pragma once

#ifdef WINVER
#undef WINVER

#endif

#define WINVER 0x0501

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define _WIN32_WINNT 0x0501

#include<winsock2.h>
#include<WS2tcpip.h>
#include<WSPiApi.h>
#include<windows.h>
#include<Psapi.h>
#include<WtsApi32.h>
#include<tchar.h>
#include<CommCtrl.h>

#pragma warning(push)
#pragma warning(disable: 4091)
#include<DbgHelp.h>
#pragma warning(pop)
#include<shellapi.h>
