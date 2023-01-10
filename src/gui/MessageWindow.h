#pragma once
#include "Utils/CommonHeader.h"
#include "WindowMessageHandler.h"

class MessageWindow
{
public:
	MessageWindow(const HINSTANCE hinst, const TCHAR* windowClassName, WindowMessageHandler* messageHandler = 0);
	virtual ~MessageWindow(void);
	virtual bool createWindow(WindowMessageHandler* messageHandler = 0);
	virtual void destroyWindow();
	HWND getHWND() const;

protected:
	virtual bool wndProc(UINT message, WPARAM wParam, LPARAM lParam);
	HWND m_hwnd;
	WindowMessageHandler* m_messageHandler;
	HINSTANCE m_hinst;
	TCHAR* m_windowClassName;

private:
	//access to memory
	//which represent some internal data structures in the system
	ATOM regClass(HINSTANCE hinst, TCHAR* windowClassName);
	static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

