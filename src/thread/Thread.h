#pragma once

#include "Utils/CommonHeader.h"
#include "LocalMutex.h"
#include "DesktopSelector.h"

enum THREAD_PRIORITY {
	PRIORITY_IDLE,
	PRIORITY_LOWEST,
	PRIORITY_BELOW_NORMAL,
	PRIORITY_NORMAL,
	PRIORITY_ABOVE_NORMAL,
	PRIORITY_HIGHEST,
	PRIORITY_TIME_CRITICAL
};

class Thread
{
public:
	Thread();
	virtual ~Thread();
	bool wait();
	bool suspend();
	bool resume();
	virtual void terminate();
	bool isActive() const;
	DWORD getThreadId() const;
	bool setPriority(THREAD_PRIORITY value);
	static void sleep(DWORD millis);
	static void yield();

protected:
	bool isTerminating();
	virtual void OnTerminate();
	virtual void execute() = 0;

private:
	static DWORD WINAPI threadProc(LPVOID pThread);
	virtual void initByDerived();

private:
	HANDLE m_hThread;
	DWORD m_threadID;
	bool m_active;
	volatile bool m_terminated;
};

