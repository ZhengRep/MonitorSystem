#include "Screen.h"
#include "Utils/Exception.h"
#include ""

Screen::Screen()
{
	update();
}

Screen::~Screen()
{
}

void Screen::update()
{
	BMI bmi;
	getBMI(&bmi, 0);

	fillPixelFormat(&bmi);
	fillScreenRect();
}

PixelFormat Screen::getPixelFormat()
{
	return m_pixelFormat;
}

Dimension Screen::getDesktopDimension()
{
	return Dimension(&m_virtDesktopRect);
}

Rect Screen::getDestopRect()
{
	return m_virtDesktopRect;
}

void Screen::getBMI(BMI* bmi, HDC dc)
{
	HDC bitmapDC = dc;
	if (bitmapDC == 0) {
		bitmapDC = GetDC(0);
		if (bitmapDC == NULL) {
			throw Exception(_T("Can't get a bitmap dc"));
		}
	}

	memset(bmi, 0, sizeof(BMI));
	bmi->bmiHeader.biBitCount = 0;
	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	HBITMAP hbm;
	hbm = (HBITMAP)GetCurrentObject(bitmapDC, OBJ_BITMAP);
	if (GetDIBits(bitmapDC, hbm, 0, 0, NULL, (LPBITMAPINFO)bmi, DIB_RGB_COLORS) == 0) {
		StringStorage errMess;
		Environment::getErrStr(_T("Can't get a DIBits"), &errMess);
		DeleteObject(hbm);
		DeleteDC(bitmapDC);
		throw Exception(errMess.getString());
	}

	// The color table is filled only if it is used BI_BITFIELDS
	if (bmi->bmiHeader.biCompression == BI_BITFIELDS) {
		if (GetDIBits(bitmapDC, hbm, 0, 0, NULL, (LPBITMAPINFO)bmi, DIB_RGB_COLORS) == 0) {
			StringStorage errMess;
			Environment::getErrStr(_T("Can't get a DIBits"), &errMess);
			DeleteObject(hbm);
			DeleteDC(bitmapDC);
			throw Exception(errMess.getString());
		}
	}

	DeleteObject(hbm);
	if (dc == 0) {
		DeleteDC(bitmapDC);
	}
}

size_t Screen::getVisibleMonitorCount()
{
	return size_t();
}

void Screen::fillPixelFormat(const BMI* bmi)
{
}

inline int Screen::findFirstBit(const UINT32 bits)
{
	return 0;
}

void Screen::fillScreenRect()
{
}
