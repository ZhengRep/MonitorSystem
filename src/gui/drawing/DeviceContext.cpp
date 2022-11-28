#include "DeviceContext.h"

DeviceContext::DeviceContext(HWND window)
    :m_hasOwnDC(false), m_wnd(window)
{
    m_dc = GetDC(window);
}

DeviceContext::DeviceContext(DeviceContext* compatibleDevice)
    :m_hasOwnDC(true)
{
    m_wnd = compatibleDevice->m_wnd;
    m_dc = CreateCompatibleDC(compatibleDevice->m_dc);
}

DeviceContext::~DeviceContext()
{
    if (m_wnd) {
        ReleaseDC(m_wnd, m_dc);
    }
    if (m_hasOwnDC) {
        DeleteDC(m_dc);
    }
}

DeviceContext::DeviceContext(PaintWindow* pntWnd)
    :m_wnd(0), m_hasOwnDC(false)
{
    m_dc = pntWnd->getHDCPaint();
}

HGDIOBJ DeviceContext::selectObject(HGDIOBJ object)
{
    return SelectObject(m_dc, object);
}
