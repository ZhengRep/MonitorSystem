#include "SolidBrush.h"

SolidBrush::SolidBrush(COLORREF color)
	:Brush()
{
	m_brush = CreateSolidBrush(color);
}

SolidBrush::~SolidBrush()
{
}
