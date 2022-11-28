#pragma once
#include "BaseWindow.h"
#include "drawing/DeviceContext.h"

class PaintWindow: public BaseWindow
{
public:
	PaintWindow();
	virtual ~PaintWindow();

private:
	HDC getHDCPaint();
	friend class DeviceContext;

protected:
	virtual void onPaint(DeviceContext* dc, PAINTSTRUCT* paintStruct);
	bool wndProc(UINT message, WPARAM wParam, LPARAM lParam);

	bool m_bIsDraw;
	PAINTSTRUCT m_paintStruct;
	HDC m_hdc;
};

