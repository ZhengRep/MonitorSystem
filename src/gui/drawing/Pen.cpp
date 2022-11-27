#include "Pen.h"

Pen::Pen(int type, int width, COLORREF color)
	:m_pen(NULL)
{
	m_pen = CreatePen(type, width, color);
}

Pen::~Pen()
{
	if (m_pen) {
		DeleteObject(m_pen);
	}
}
