#include "WindowsEvent.h"
#include "thread/Thread.h"
#include "Utils/Exception.h"

WindowsEvent::WindowsEvent(const TCHAR* name)
{
	m_hEvent = CreateEvent(0, FALSE, FALSE, name);
	if (m_hEvent == 0) {
		int errCode = GetLastError();
		StringStorage errMsg;
		errMsg.format(_T("Cannot create windows event with error = %d"), errCode);
		throw Exception(errMsg.getString());
	}
}

WindowsEvent::~WindowsEvent()
{
	if (m_hEvent) {
		CloseHandle(m_hEvent);
	}
}

void WindowsEvent::notify()
{
	if (m_hEvent) {
		SetEvent(m_hEvent);
	}
}

void WindowsEvent::waitForEvent(DWORD milliSeconds)
{
	if (m_hEvent) {
		WaitForSingleObject(m_hEvent, milliSeconds);
	}
}
