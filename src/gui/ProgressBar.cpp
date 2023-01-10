#include "ProgressBar.h"

ProgressBar::ProgressBar()
{
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::setRange(WORD min, WORD max)
{
	SendMessage(m_hwnd, PBM_SETRANGE, 0, MAKELPARAM(min, max));
}

void ProgressBar::setPos(WORD pos)
{
	SendMessage(m_hwnd, PBM_SETPOS, (WPARAM)pos, 0);
}

