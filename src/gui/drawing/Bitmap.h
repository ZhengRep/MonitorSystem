#pragma once
#include "Utils/CommonHeader.h"


class Bitmap
{
public:
	Bitmap(int width, int height);
	//Creates compatibla with dc bitmap
	Bitmap(HDC dc, int width, int height);
	Bitmap(HBITMAP bitmap);
	virtual ~Bitmap();

	int getWidth() const;
	int getHeight() const;

protected:
	HBITMAP m_bitmap;

	friend class Icon;
	friend class Graphics;
	friend class BitmapGraphics;
};

