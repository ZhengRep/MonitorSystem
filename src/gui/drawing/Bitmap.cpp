#include "Bitmap.h"

Bitmap::Bitmap(int width, int height)
	:m_bitmap(NULL)
{
	//prepare buffer
	int bpp = 32; //bit per pixel
	size_t size = width * height * (bpp / 8);
	std::vector<unsigned char> bits(size);
	if (width != 0 && height != 0) {
		memset(&bits.front(), 0, size);
		m_bitmap = CreateBitmap(width, height, 1, bpp, &bits.front());
	}
}

Bitmap::Bitmap(HDC dc, int width, int height)
{
	m_bitmap = CreateCompatibleBitmap(dc, width, height);
}

Bitmap::Bitmap(HBITMAP bitmap)
	:m_bitmap(bitmap)
{
}

Bitmap::~Bitmap()
{
	if (m_bitmap) {
		DeleteObject(m_bitmap);
	}
}

int Bitmap::getWidth() const
{
	BITMAP bitmap;
	if (GetObject(m_bitmap, sizeof(BITMAP), &bitmap) == 0) {
		return 0;
	}
	return bitmap.bmWidth;
}

int Bitmap::getHeight() const
{
	BITMAP bitmap;
	if (GetObject(m_bitmap, sizeof(BITMAP), &bitmap) == 0) {
		return 0;
	}
	return bitmap.bmHeight;
}
