#include "Graphics.h"

Graphics::Graphics(DeviceContext* dc)
	:m_dc(dc)
{
}

Graphics::~Graphics()
{
}

void Graphics::setBkMode(bool transparent)
{
	SetBkMode(m_dc->m_dc, transparent ? TRANSPARENT : OPAQUE);
}

void Graphics::setBkColor(COLORREF color)
{
	SetBkColor(m_dc->m_dc, color);
}

void Graphics::setTextColor(COLORREF color)
{
	SetTextColor(m_dc->m_dc, color);
}

void Graphics::setBrush(const Brush* brush)
{
	HGDIOBJ object = brush ? brush->m_brush : 0;
	m_dc->selectObject(object);
}

void Graphics::setPen(const Pen* pen)
{
	HGDIOBJ object = pen ? pen->m_pen : 0;
	m_dc->selectObject(object);
}

void Graphics::moveTo(int x, int y)
{
	MoveToEx(m_dc->m_dc, x, y, NULL);
}

void Graphics::lineTo(int x, int y)
{
	LineTo(m_dc->m_dc, x, y);
}

void Graphics::fillRect(int l, int t, int r, int b, const Brush* brush)
{
	RECT rect;
	rect.left = l;
	rect.top = t;
	rect.right = r;
	rect.bottom = b;
	FillRect(m_dc->m_dc, &rect, brush->m_brush);
}

void Graphics::ellipse(int l, int t, int r, int b)
{
	Ellipse(m_dc->m_dc, l, t, r, b);
}

void Graphics::rectangle(int l, int t, int r, int b)
{
	Rectangle(m_dc->m_dc, l, t, r, b);
}

void Graphics::drawBitmap(const Bitmap* bitmap, int x, int y, int w, int h)
{
	DeviceContext memDC(m_dc);
	HGDIOBJ oldBitmap = memDC.selectObject(bitmap->m_bitmap);
	BitBlt(m_dc->m_dc, x, y, w, h, memDC.m_dc, 0, 0, SRCCOPY);
	memDC.selectObject(oldBitmap);
}

void Graphics::drawText(const TCHAR* text, int cchText, RECT* rect, UINT format)
{
	DrawText(m_dc->m_dc, text, cchText, rect, format);
}
