#include "WTS.h"
#include "SystemException.h"
#include<crtdbg.h>
#include "thread/AutoLock.h"
#include ""

DWORD WTS::getActiveConsoleSessionId(LogWriter* log)
{
    return 0;
}

DWORD WTS::getRdpSessionId(LogWriter* log)
{
    return 0;
}

bool WTS::SessionIsRdpSession(DWORD sessionId)
{
    return false;
}

void WTS::queryConsoleUserToken(HANDLE* token, LogWriter* log) throw(SystemException)
{
}

void WTS::defineConsoleUserProcessId(DWORD userProcessId)
{
}

void WTS::duplicatePipeClientToken(HANDLE pipeHandle)
{
}

bool WTS::getCurrentUserName(StringStorage* userName, LogWriter* log)
{
    return false;
}

WTS::WTS()
{
}

void WTS::initialize(LogWriter* log)
{
}

void WTS::wtsFreeMemory(void* buffer)
{
}
