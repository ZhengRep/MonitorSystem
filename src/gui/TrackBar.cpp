#include "TrackBar.h"

TrackBar::TrackBar()
{
}

TrackBar::~TrackBar()
{
}

void TrackBar::setRange(long min, long max)
{
    SendMessage(m_hwnd, TBM_SETRANGEMIN, FALSE, min);
    SendMessage(m_hwnd, TBM_SETRANGEMAX, TRUE, max);
}

void TrackBar::setPos(long pos)
{
    SendMessage(m_hwnd, TBM_SETPOS, TRUE, pos);
}

long TrackBar::getPos()
{
    return SendMessage(m_hwnd, TBM_GETPOS, 0, 0);
}
