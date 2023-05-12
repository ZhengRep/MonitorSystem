#pragma once

#include "utils/winhdr.h"
#include "thread/LocalMutex.h"
#include "DynamicLibrary.h"
#include "SystemException.h"
#include "log_writer/LogWriter.h"

//Windows Terminal Server
typedef DWORD(WINAPI* pWTSGetActiveConsoleSessionId)(void);

typedef BOOL(WINAPI* pWTSQueryUserToken)(ULONG SessionId, PHANDLE phToken);

typedef BOOL(WINAPI* pWTSQuerySessionInformationW)(HANDLE hServer, DWORD SessionId,
	WTS_INFO_CLASS WTSInfoClass,LPWSTR** ppBuffer,DWORD* pBytesReturned);

typedef BOOL(WINAPI* pWTSQuerySessionInformationA)(HANDLE hServer, DWORD SessionId,
	WTS_INFO_CLASS WTSInfoClass,LPSTR** ppBuffer,DWORD* pBytesReturned);

typedef VOID(WINAPI* pWTSFreeMemory)(void* buffer);
typedef BOOL(WINAPI* pWTSEnumerateSessionsA)(IN HANDLE hServer,IN DWORD Reserved,
	IN DWORD Version, PWTS_SESSION_INFOA* ppSessionInfo,DWORD* pCount);

typedef BOOL(WINAPI* pWTSEnumerateSessionsW)(IN HANDLE hServer,IN DWORD Reserved,
	IN DWORD Version,PWTS_SESSION_INFOW* ppSessionInfo,DWORD* pCount
	);

#ifdef UNICODE
#define   pWTSQuerySessionInformation pWTSQuerySessionInformationW
#define   pWTSEnumerateSessions pWTSEnumerateSessionsW
#else
#define pWTSQuerySessionInformation pWTSQuerySessionInformationA
#define pWTSEnumerateSessions pWTSEnumerateSessionsA
#endif

class WTS
{
public:
	static DWORD getActiveConsoleSessionId(LogWriter* log);
	static DWORD getRdpSessionId(LogWriter* log);
	static bool SessionIsRdpSession(DWORD sessionId);
	static void queryConsoleUserToken(HANDLE* token, LogWriter* log) throw(SystemException);
	static void defineConsoleUserProcessId(DWORD userProcessId);
	static void duplicatePipeClientToken(HANDLE pipeHandle);
	static bool getCurrentUserName(StringStorage* userName, LogWriter* log);

private:
	WTS();
	static void initialize(LogWriter* log);

	// The initialize() function should be already called before use the wtsFreeMemory() function.
	static void wtsFreeMemory(void* buffer);

	static DynamicLibrary* m_kernel32Library;
	static DynamicLibrary* m_wtsapi32Library;
	static pWTSGetActiveConsoleSessionId m_WTSGetActiveConsoleSessionId;
	static pWTSQueryUserToken m_WTSQueryUserToken;
	static pWTSQuerySessionInformation m_WTSQuerySessionInformation;
	static pWTSFreeMemory m_WTSFreeMemory;
	static pWTSEnumerateSessions m_WTSEnumerateSessions;

	static HANDLE m_userProcessToken;
	static volatile bool m_initialized;
	static LocalMutex m_mutex;
};

