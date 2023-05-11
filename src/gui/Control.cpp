#include "Control.h"

Control::Control()
    :m_hwnd(NULL)
{
}

Control::Control(HWND hwnd)
    :m_hwnd(hwnd)
{
}

Control::~Control()
{
}

void Control::setWindow(HWND hwnd)
{
    m_hwnd = hwnd;
    m_defWindowProc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
}

void Control::setEnabled(bool enabled)
{
    if (enabled) {
        SendMessage(m_hwnd, WM_ENABLE, TRUE, NULL);
        removeStyle(WS_DISABLED);
    }
    else {
        if (isStyleEnabled(WS_DISABLED)) {
            return;
        }
        SendMessage(m_hwnd, WM_ENABLE, TRUE, NULL);
        addStyle(WS_DISABLED);
    }
    invalidate();
}

void Control::setText(const TCHAR* text)
{
    SetWindowText(m_hwnd, text);
}

void Control::setSignedInt(int value)
{
    StringStorage text;
    text.format(_T("%d"), value);
    setText(text.getString());
}

void Control::setUnsignedInt(unsigned int value)
{
    StringStorage text;
    text.format(_T("%u"), value);
    setText(text.getString());
}

void Control::setTextVerticalAlinment(VerticalAlignment align)
{

}

void Control::setFocus()
{
    SetFocus(m_hwnd);
}

bool Control::hasFocus()
{
    return (GetFocus() == m_hwnd) || (GetForegroundWindow() == m_hwnd);
}

bool Control::setForeground()
{
    return SetForegroundWindow(m_hwnd) != 0; //BOOL -> bool
}

void Control::setVisiable(bool visiable)
{
    ShowWindow(m_hwnd, visiable ? SW_SHOW : SW_HIDE);
}

bool Control::isEnabled()
{
    return !isStyleEnabled(WS_DISABLED);
}

void Control::invalidate()
{
    //NULL: the entire client area is added to the update region
    InvalidateRect(m_hwnd, NULL, TRUE);
}

void Control::getText(StringStorage* storage)
{
    int length = SendMessage(m_hwnd, WM_GETTEXTLENGTH, 0, 0);
    std::vector<TCHAR> buf(length + 1);
    GetWindowText(m_hwnd, &buf.front(), length + 1);
    storage->setString(&buf.front());
}

VerticalAlignment Control::getTextverticalAlignment()
{
    return Left;
}

HWND Control::getWindow()
{
    return m_hwnd;
}

void Control::setStyle(DWORD styleFlags)
{
    SetWindowLong(m_hwnd, GWL_STYLE, styleFlags);
}

DWORD Control::getStyle()
{
    return GetWindowLong(m_hwnd, GWL_STYLE);
}

void Control::addStyle(DWORD styleFlag)
{
    DWORD flags = getStyle();
    flags |= styleFlag;
    setStyle(flags);
}

void Control::removeStyle(DWORD styleFlag)
{
    DWORD flags = getStyle();
    flags &= ~styleFlag;
    setStyle(flags);
}

bool Control::isStyleEnabled(DWORD styleFlags)
{
    DWORD flags = getStyle();
    return (flags & styleFlags) == styleFlags;
}

void Control::setExStyle(DWORD style)
{
    SetWindowLong(m_hwnd, GWL_EXSTYLE, style);
}

void Control::addExStyle(DWORD styleFlag)
{
    DWORD flags = getExStyle();
    flags |= styleFlag;
    setExStyle(flags);
}

void Control::removeExStyle(DWORD styleFlag)
{
    DWORD flags = getExStyle();
    flags &= ~styleFlag;
    setExStyle(flags);
}

DWORD Control::getExStyle()
{
    return GetWindowLong(m_hwnd, GWL_EXSTYLE);
}

bool Control::isExStyleEnalbed(DWORD styleFlag)
{
    DWORD flags = getExStyle();
    return (flags & styleFlag) == styleFlag;
}

void Control::replaceWindowProc(WNDPROC wndProc)
{
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
    SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG_PTR)wndProc);
}
