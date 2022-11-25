#include "LocalMutex.h"

LocalMutex::LocalMutex()
{
	InitializeCriticalSection(&m_criticalSection);
}

LocalMutex::~LocalMutex()
{
	DeleteCriticalSection(&m_criticalSection);
}

void LocalMutex::lock()
{
	EnterCriticalSection(&m_criticalSection);
}

void LocalMutex::unlock()
{
	LeaveCriticalSection(&m_criticalSection);
}
