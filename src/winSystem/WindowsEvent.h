#pragma once

#include "Utils/CommonHeader.h"

class WindowsEvent
{
public:
	WindowsEvent(const TCHAR* name = 0);
	virtual ~WindowsEvent();

	void notify();
	void waitForEvent(DWORD milliSeconds = INFINITE);
	HANDLE getHandle() const { return m_hEvent; };

protected:
	HANDLE m_hEvent;

};

