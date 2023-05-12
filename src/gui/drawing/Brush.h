#pragma once
#include "utils/CommonHeader.h"

class Brush
{
public:
	Brush();
	virtual ~Brush();

protected:
	void release();

protected:
	HBRUSH m_brush;

	friend class Graphics;
};

