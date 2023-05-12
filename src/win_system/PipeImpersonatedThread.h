#pragma once

#include "thread/Thread.h"
#include "WindowsEvent.h"

class PipeImpersonatedThread: public Thread
{
public:
	PipeImpersonatedThread(HANDLE pipeHandle);
	virtual ~PipeImpersonatedThread();

	void waitUntilImpersonated();
	bool getImpersonationSuccess();
	void getFaultReason(StringStorage* faultReason);

private:
	virtual void execute();
	virtual void onTerminate();

	HANDLE m_pipeHandle;
	bool m_success;
	StringStorage m_faultReason;
	WindowsEvent m_impersonationReadyEvent;
	WindowsEvent m_threadSleeper;
};

