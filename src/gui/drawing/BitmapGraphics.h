#pragma once
#include "Bitmap.h"
#include "Graphics.h"

//Object that creates bitmap with specified size and allow to draw into 
//in through Graphics class methods
class BitmapGraphics: public Graphics
{
public:
	BitmapGraphics(DeviceContext* compatibleDC, int width, int height);
	// Destroys bitmap and graphics object.
	virtual ~BitmapGraphics();

	// Returns target bitmap that we paint.
	Bitmap* getBitmap();

	void beginPaint();
	void endPaint();

protected:
	bool m_isPainting;
	Bitmap* m_bitmap;
	HGDIOBJ m_oldBitmap;
};

