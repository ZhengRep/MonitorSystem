#include "Brush.h"

Brush::Brush()
	:m_brush(0)
{
}

Brush::~Brush()
{
	release();
}

void Brush::release()
{
	if (m_brush) {
		DeleteObject(m_brush);
		m_brush = 0;
	}
}
