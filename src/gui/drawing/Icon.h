#pragma once
#include "Bitmap.h"

class Icon
{
public:
	Icon();
	Icon(HICON icon);
	Icon(Bitmap* bitmap);
	Icon(Bitmap* bitmap, Bitmap* mask);
	Icon(DWORD icon);
	virtual ~Icon();

	HICON getHICON();

protected:
	void fromBitmap(Bitmap* bitmap, Bitmap* mask);

protected:
	HICON m_icon;
	bool m_hasOwnIcon;
};

