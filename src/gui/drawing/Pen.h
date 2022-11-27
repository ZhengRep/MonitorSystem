#pragma once
#include "Utils/CommonHeader.h"

class Pen
{
public:
	const static int SOLID_TYPE = PS_SOLID;
	const static int DASH_TYPE = PS_DASH;
	const static int NULL_TYPE = PS_NULL;

public:
	Pen(int type, int width, COLORREF color);
	virtual ~Pen();

protected:
	HPEN m_pen;

	friend class Graphics;
};

