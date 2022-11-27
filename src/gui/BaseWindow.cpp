#include "BaseWindow.h"

BaseWindow::BaseWindow()
    :m_hWnd(0),
    m_hicon(0),
    m_bWndCreated(false)
{
}

BaseWindow::~BaseWindow()
{
    if (m_bWndCreated && m_hWnd) {
        DestroyWindow(m_hWnd);
    }
    if (m_hicon) {
        DeleteObject(m_hicon);
    }
}

HWND BaseWindow::getHWnd() const
{
    return m_hWnd;
}

void BaseWindow::setHWnd(HWND hwnd)
{
    m_hWnd = hwnd;
}

bool BaseWindow::createWindow(const StringStorage* windowName, DWORD style, HWND hWndParent, 
    int xPos, int yPos, int width, int height)
{
    if (m_hWnd) {
        return false;
    }
    m_windowName = *windowName;
    m_hWnd = CreateWindow(m_className.getString(), m_windowName.getString(), style,
        xPos, yPos, width, height, hWndParent, 0, GetModuleHandle(0), this);

    m_bWndCreated = (m_hWnd == 0) ? false : true;
    if (m_bWndCreated) {
        SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)); //Test this ???
    }
    return true;
}

bool BaseWindow::destroyWindow()
{
    if (m_hWnd) {
        DestroyWindow(m_hWnd);
        return true;
    }
    return false;
}

void BaseWindow::setClass(const StringStorage* className)
{
    m_className = *className;
}

void BaseWindow::show()
{
    _ASSERT(m_hWnd);
    ShowWindow(m_hWnd, SW_SHOW);
}

void BaseWindow::hide()
{
    _ASSERT(m_hWnd);
    ShowWindow(m_hWnd, SW_HIDE);
}

void BaseWindow::enableWindow(bool bEnable)
{
    _ASSERT(m_hWnd);
    EnableWindow(m_hWnd, bEnable);
}

void BaseWindow::updateWindow()
{
    _ASSERT(m_hWnd);
    UpdateWindow(m_hWnd);
}

bool BaseWindow::setSize(int width, int height)
{
    _ASSERT(m_hWnd);
    return !!SetWindowPos(m_hWnd, 0, 0, 0, 
        width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

bool BaseWindow::setPosition(int xPos, int yPos)
{
    _ASSERT(m_hWnd);
    return !!SetWindowPos(m_hWnd, 0, xPos, yPos,
        0, 0, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void BaseWindow::setWindowText(const StringStorage* text)
{
    _ASSERT(m_hWnd);
    SetWindowText(m_hWnd, text->getString());
}

void BaseWindow::loadIcon(DWORD id)
{
    if (m_hicon) {
        DeleteObject(m_hicon);
        m_hicon = 0;
    }
    if (IS_INTRESOURCE(id)) {
        m_hicon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id));
        SetClassLongPtr(m_hWnd, GCLP_HICON, reinterpret_cast<LONG_PTR>(m_hicon));
    }
    else {
        SetClassLongPtr(m_hWnd, GCLP_HICON, static_cast<LONG_PTR>(id));
    }
}

void BaseWindow::setParent(HWND hwnd)
{
    _ASSERT(m_hWnd);
    SetParent(m_hWnd, hwnd);
}

void BaseWindow::setClassStyle(DWORD style)
{
    _ASSERT(m_hWnd);
    SetClassLong(m_hWnd, GCL_STYLE, style);
}

void BaseWindow::setClassCursor(HCURSOR hcursor)
{
    _ASSERT(m_hWnd);
    SetClassLongPtr(m_hWnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(hcursor));
}

void BaseWindow::setClassBackground(HBRUSH hbrush)
{
    _ASSERT(m_hWnd);
    SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(hbrush));
}

void BaseWindow::setClassMenu(LONG menu)
{
    _ASSERT(m_hWnd);
    SetClassLongPtr(m_hWnd, GCLP_MENUNAME, menu);
}

LONG BaseWindow::getStyle()
{
    _ASSERT(m_hWnd);
    return GetWindowLong(m_hWnd, GWL_STYLE);
}

void BaseWindow::setStyle(DWORD style)
{
    _ASSERT(m_hWnd);
    SetClassLong(m_hWnd, GCL_STYLE, style);
}

LONG BaseWindow::getExStyle()
{
    _ASSERT(m_hWnd);
    return GetWindowLong(m_hWnd, GWL_EXSTYLE);
}

void BaseWindow::setExStyle(DWORD exstyle)
{
    _ASSERT(m_hWnd);
    SetWindowLong(m_hWnd, GWL_EXSTYLE, exstyle);
}

void BaseWindow::redraw(const RECT* rcArea)
{
    _ASSERT(m_hWnd);
    if (rcArea == 0) {
        InvalidateRect(m_hWnd, NULL, TRUE); //Test this ??? erase window paint buffer and erase original
    }
    else {
        InvalidateRect(m_hWnd, rcArea, FALSE);
    }
}

void BaseWindow::setTimer(UINT_PTR ident, UINT time)
{
    _ASSERT(m_hWnd);
    SetTimer(m_hWnd, ident, time, 0);
}

void BaseWindow::killTimer(UINT_PTR ident)
{
    _ASSERT(m_hWnd);
    KillTimer(m_hWnd, ident);
}

void BaseWindow::setForegroundWindow()
{
    _ASSERT(m_hWnd);
    SetForegroundWindow(m_hWnd);
}

void BaseWindow::postMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    _ASSERT(m_hWnd);
    PostMessage(m_hWnd, msg, wParam, lParam);
}

void BaseWindow::getClientRect(RECT* rc)
{
    _ASSERT(m_hWnd && rc);
    GetClientRect(m_hWnd, rc);
}

void BaseWindow::getBorderSize(int* width, int* height)
{
    _ASSERT(m_hWnd);
    *width = 2 * GetSystemMetrics(SM_CXSIZEFRAME);
    *height = GetSystemMetrics(SM_CYSIZE) + 2 * GetSystemMetrics(SM_CYSIZEFRAME);
}

bool BaseWindow::wndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_COMMAND:
        return onCommand(wParam, lParam);
    case WM_NOTIFY:
        return onNotify(wParam, (LPNMHDR)lParam);
    case WM_SYSCOMMAND:
        return onSysCommand(wParam, lParam);
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_MOUSEMOVE:
    {
        unsigned char mouseButtons = 0;

        mouseButtons |= LOWORD(wParam) & MK_RBUTTON ? MOUSE_RDOWN : 0;
        mouseButtons |= LOWORD(wParam) & MK_MBUTTON ? MOUSE_MDOWN : 0;
        mouseButtons |= LOWORD(wParam) & MK_LBUTTON ? MOUSE_LDOWN : 0;
        
        //translate position from LPARAM to POINT
        POINTS points = MAKEPOINTS(lParam);
        POINT point;
        point.x = points.x;
        point.y = points.y;

        unsigned short wheelSpeed = 0;
        if (message == WM_MOUSEWHEEL) {
            //Get speed wheel and set mouse button
            signed short wheelSignedSpeed = static_cast<signed short>(HIWORD(wParam));
            if (wheelSignedSpeed < 0) {
                mouseButtons |= MOUSE_WDOWN;
                wheelSpeed = -wheelSignedSpeed / WHEEL_DELTA;
            }
            else {

            }
        }
    }
    }
}

bool BaseWindow::onCommand(WPARAM wParam, LPARAM lParam)
{
    return false;
}

bool BaseWindow::onNotify(int idCtrl, LPNMHDR pnmh)
{
    return false;
}

bool BaseWindow::onSysCommand(WPARAM wParam, LPARAM lParam)
{
    return false;
}

bool BaseWindow::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    return false;
}

bool BaseWindow::onMouse(unsigned char mouseButtons, unsigned wheelSpeed, POINT position)
{
    return false;
}
