#include "PaintWindow.h"

PaintWindow::PaintWindow()
	:m_bIsDraw(false)
{
}

PaintWindow::~PaintWindow()
{
}

HDC PaintWindow::getHDCPaint()
{
	if (m_bIsDraw) {
		return m_hdc;
	}
	return 0;
}

void PaintWindow::onPaint(DeviceContext* dc, PAINTSTRUCT* paintStruct)
{
}

bool PaintWindow::wndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_PAINT) {
		m_hdc = BeginPaint(m_hWnd, &m_paintStruct);
		m_bIsDraw = true;
		DeviceContext dc(this);
		onPaint(&dc, &m_paintStruct);
		EndPaint(m_hWnd, &m_paintStruct);
		m_bIsDraw = false;
		return true;
	}
	return BaseWindow::wndProc(message, wParam, lParam);
}
