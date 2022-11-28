#pragma once
#include "DeviceContext.h"
#include "Bitmap.h"
#include "Pen.h"
#include "Brush.h"

class Graphics
{
public:
	Graphics(DeviceContext* dc);
	virtual ~Graphics();

	//Sets background colors mix mode
	void setBkMode(bool transparent);
	void setBkColor(COLORREF color);
	void setTextColor(COLORREF color);
	void setBrush(const Brush* brush);
	void setPen(const Pen* pen);

	void moveTo(int x, int y);
	void lineTo(int x, int y);
	void fillRect(int l, int t, int r, int b, const Brush* brush);
	void ellipse(int l, int t, int r, int b);
	void rectangle(int l, int t, int r, int b);

	void drawBitmap(const Bitmap* bitmap, int x, int y, int w, int h);
	void drawText(const TCHAR* text, int cchText, RECT* rect, UINT format);

protected:
	DeviceContext* m_dc;
};

