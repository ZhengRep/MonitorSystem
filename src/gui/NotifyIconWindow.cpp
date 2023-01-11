#include "NotifyIconWindow.h"

NotifyIconWindow::NotifyIconWindow()
    :m_wph(0)
{
    WNDCLASS wc;
    wc.style = 0;
    wc.lpfnWndProc = WindowProcHolder::defWindowProc;
    wc.cbClsExtra = 0;
    wc.hInstance = GetModuleHandle(0);
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;//hBrush
    wc.lpszMenuName = NULL;
    wc.lpszClassName = _T("NotifyIconWindowClass");

    ATOM atom = RegisterClass(&wc);

    m_window = CreateWindow((LPCTSTR)atom, _T("NotifyIconWindowTitle"), WS_OVERLAPPED, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(0), NULL);

    SetWindowLongPtr(m_window, GWLP_USERDATA, (LONG_PTR)m_wph);
}

NotifyIconWindow::~NotifyIconWindow()
{
    setWindowProcHolder(NULL);
}

HWND NotifyIconWindow::getWindow()
{
    return m_window;
}

void NotifyIconWindow::setWindowProcHolder(WindowProcHolder* wph)
{
    m_wph = wph;
    SetWindowLongPtr(m_window, GWLP_USERDATA, (LONG_PTR)m_wph);
}
