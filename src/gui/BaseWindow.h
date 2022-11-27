#pragma once
#include "Utils/CommonHeader.h"

class BaseWindow
{
public:
	BaseWindow();
	virtual ~BaseWindow();

	//Get a handle of the window
	HWND getHWnd() const;
	void setHWnd(HWND hwnd);

	//create window with windowName and setted style
	bool createWindow(const StringStorage* windowName, DWORD style, HWND hWndParent = 0,
		int xPos = CW_USEDEFAULT, int yPos = CW_USEDEFAULT,
		int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);
	bool destroyWindow();

	//Set a class name only to the new window created by createWindow
	void setClass(const StringStorage* className);

	void show();
	void hide();
	void enableWindow(bool bEnable);
	void updateWindow();
	bool setSize(int width, int height);
	bool setPosition(int xPos, int yPos);
	void setWindowText(const StringStorage* text);
	void loadIcon(DWORD id);
	//Making child window by changing parent of the window
	void setParent(HWND hwnd);

	//for changing registered class parameters of created window
	void setClassStyle(DWORD style);
	void setClassCursor(HCURSOR hcursor);
	void setClassBackground(HBRUSH hbrush);
	void setClassMenu(LONG menu);

	//for changing or get style and exstyle of window
	LONG getStyle();
	void setStyle(DWORD style);
	LONG getExStyle();
	void setExStyle(DWORD exstyle);

	//full redraw of window area
	void redraw(const RECT* rcArea = 0);

	void setTimer(UINT_PTR ident, UINT time);
	void killTimer(UINT_PTR ident);

	void setForegroundWindow();

	void postMessage(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);
	void getClientRect(RECT* rc);
	void getBorderSize(int* width, int* height);

	virtual bool wndProc(UINT message, WPARAM wParam, LPARAM lParam);

	static const int MOUSE_LDOWN = 1;
	static const int MOUSE_MDOWN = 2;
	static const int MOUSE_RDOWN = 4;
	static const int MOUSE_WUP = 8;
	static const int MOUSE_WDOWN = 16;

private:
	virtual bool onCommand(WPARAM wParam, LPARAM lParam);
	virtual bool onNotify(int idCtrl, LPNMHDR pnmh);
	virtual bool onSysCommand(WPARAM wParam, LPARAM lParam);
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool onMouse(unsigned char mouseButtons, unsigned wheelSpeed, POINT position);

protected:
	HWND m_hWnd;
	StringStorage m_className;
	StringStorage m_windowName;
	HICON m_hicon;
	bool m_bWndCreated;

};

