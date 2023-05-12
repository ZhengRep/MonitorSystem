#pragma once
#include "utils/CommonHeader.h"
#include "gui/PaintWindow.h"

class DeviceContext
{
public:
	DeviceContext(HWND window);
	DeviceContext(DeviceContext* compatibleDevice);
	virtual ~DeviceContext();

private:
	//Initialize class from PaintWindow
	DeviceContext(class PaintWindow* pntWnd);
	friend class PaintWindow;

protected:
	//selects an object into this device context
	HGDIOBJ selectObject(HGDIOBJ object);

protected:
	HDC m_dc;
	HWND m_wnd;
	bool m_hasOwnDC;

	friend class Graphics;
	friend class BitmapGraphics;
};

