#pragma once
#include "Utils/CommonHeader.h"
#include "WindowProcHolder.h"

class NotifyIconWindow
{
public:
	NotifyIconWindow();
	virtual ~NotifyIconWindow();

	HWND getWindow();
	void setWindowProcHolder(WindowProcHolder* wph);

protected:
	HWND m_window;
	WindowProcHolder* m_wph;
	friend class NotifyIcon;
};

