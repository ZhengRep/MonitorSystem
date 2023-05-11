#pragma once
#include "Utils/CommonHeader.h"

enum VerticalAlignment
{
	Left = 0x0,
	Center = 0x1,
	Right = 0x2
};

//Base class to control windows control
class Control
{
public:
	Control();
	Control(HWND hwnd);
	virtual ~Control();

	//Sets HWND associated with this control
	void setWindow(HWND hwnd);

	virtual void setEnabled(bool enabled);
	virtual void setText(const TCHAR* text);
	virtual void setSignedInt(int value);
	virtual void setUnsignedInt(unsigned int value);

	void setTextVerticalAlinment(VerticalAlignment align);
	void setFocus();
	bool hasFocus();
	bool setForeground();
	void setVisiable(bool visiable);

	virtual bool isEnabled();
	//Invalidates control
	void invalidate();
	
	//Gets text associate with window
	virtual void getText(StringStorage* storage);
	VerticalAlignment getTextverticalAlignment();

	HWND getWindow();
	void operator = (const HWND& window) { setWindow(window); };

protected:
	void setStyle(DWORD styleFlags);
	DWORD getStyle();
	void addStyle(DWORD styleFlag);
	void removeStyle(DWORD styleFlag);
	bool isStyleEnabled(DWORD styleFlags);
	void setExStyle(DWORD style);
	void addExStyle(DWORD styleFlag);
	void removeExStyle(DWORD styleFlag);
	DWORD getExStyle();
	bool isExStyleEnalbed(DWORD styleFlag);

	/*
	Replaces default window proc with wndProc
	This method must be called by child classes if needs to override default window behavour
	*/
	void replaceWindowProc(WNDPROC wndProc);

protected:
	HWND m_hwnd;
	WNDPROC m_defWindowProc;
};


