#pragma once
#include "Control.h"
#include "ThemeLib.h"

class ImageButton: public Control
{
public:
	ImageButton();
	~ImageButton();

	//Draws this owner-draw button
	void drawItem(LPDRAWITEMSTRUCT dis);
	virtual void setWindow(HWND hwnd);
	void setIcon(HICON* icon, int width, int height);

private:
	void calcRect(RECT* buttonRect, bool isButtonPressed, DWORD textWidth, DWORD textHeight,
		DWORD imageWidth, DWORD imageHeight, RECT* textRect, RECT* imageRect);
	void drawIcon(HDC* dc, RECT* imageRect, bool isPressed, bool isDisabled);

protected:
	bool m_isUsingTheme;
	bool m_mouseOver;
	HTHEME m_theme;
	HICON* m_icon;
	int m_iconWidth;
	int m_iconHeight;

private:
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

