#pragma once
#include "Utils/CommonHeader.h"

class WindowProcHolder
{
public:
	WindowProcHolder();
	virtual ~WindowProcHolder();

protected:
	virtual LRESULT windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* useDefWindowProc) = 0;
	static LRESULT CALLBACK defWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend class NotifyIconWindow;
};

