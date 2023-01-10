#include "MessageWindow.h"

#define DEFAULT_WINDOW_CLASS_NAME "WindowClass"

MessageWindow::MessageWindow(const HINSTANCE hinst, const TCHAR* windowClassName, WindowMessageHandler* messageHandler)
    :m_hwnd(0),
    m_hinst(hinst),
    m_windowClassName(0),
    m_messageHandler(messageHandler)
{
    if (windowClassName != 0) {
        m_windowClassName = _tcsdup(windowClassName);
    }
    else {
        m_windowClassName = _tcsdup(_T(DEFAULT_WINDOW_CLASS_NAME));
    }
}

MessageWindow::~MessageWindow(void)
{
    destroyWindow();

    if (m_windowClassName != 0) {
        UnregisterClass(m_windowClassName, m_hinst);
        free(m_windowClassName);
    }
}

bool MessageWindow::createWindow(WindowMessageHandler* messageHandler)
{
    return false;
}

void MessageWindow::destroyWindow()
{
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = 0;
    }
}

HWND MessageWindow::getHWND() const
{
    return HWND();
}

bool MessageWindow::wndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    return false;
}

ATOM MessageWindow::regClass(HINSTANCE hinst, TCHAR* windowClassName)
{
    return ATOM();
}

LRESULT MessageWindow::staticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}
