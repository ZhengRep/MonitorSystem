#pragma once

#include "utils/CommonHeader.h"
#include "rfb/PixelFormat.h"
#include "region/Rect.h"
#include "region/Dimension.h"

//get info for a window desktop
class Screen
{
public:
	Screen();
	~Screen();

	void update();

	PixelFormat getPixelFormat();
	Dimension getDesktopDimension();
	Rect getDesktopRect();
	
	struct BMI
	{
		BITMAPINFOHEADER bmiHeader;
		UINT32 red;
		UINT32 green;
		UINT32 blue;
	};

	struct Palette8bitBMI
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD rgbQuad[256];
	};

	void getBMI(BMI* bmi, HDC dc);

	size_t getVisibleMonitorCount();

private:
	void fillPixelFormat(const BMI* bmi);
	static inline int findFirstBit(const UINT32 bits);
	void fillScreenRect();
	PixelFormat m_pixelFormat;
	Rect m_virtDesktopRect;
};

