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
    if (messageHandler != 0) {
        m_messageHandler = messageHandler;
    }
    if (regClass(m_hinst, m_windowClassName) == 0) {
        return false;
    }

    m_hwnd = ::CreateWindow(m_windowClassName, _T("MessageWindow"), WS_OVERLAPPEDWINDOW,
        0, 0, 1, 1, 0, NULL, m_hinst, this);
    if (m_hwnd == 0) {
        return false;
    }

    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
    return true;
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
    return m_hwnd;
}

bool MessageWindow::wndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    return true;
}

ATOM MessageWindow::regClass(HINSTANCE hinst, TCHAR* windowClassName)
{
    WNDCLASS wcWindowClass = { 0 };
    wcWindowClass.lpfnWndProc = staticWndProc;
    wcWindowClass.style = NULL;
    wcWindowClass.hInstance = m_hinst;
    wcWindowClass.hCursor = NULL;
    wcWindowClass.lpszClassName = windowClassName;
    wcWindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

    return RegisterClass(&wcWindowClass);
}

LRESULT MessageWindow::staticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MessageWindow* _this;
    if (message == WM_CREATE) {
        _this = (MessageWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
        wParam = (WPARAM)hwnd;
    }
    else {
        _this = (MessageWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (_this != NULL) {
        bool result;
        if (_this->m_messageHandler != 0) {
            result = _this->m_messageHandler->processMessage(message, wParam, lParam);
        }
        else {
            result = _this->wndProc(message, wParam, lParam);
        }
        if (result) {
            return 0;
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
