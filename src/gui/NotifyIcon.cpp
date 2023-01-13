#include "NotifyIcon.h"

NotifyIcon::NotifyIcon(bool showAfterCreation)
    :NotifyIconWindow(),
    m_icon(0),
    m_visible(showAfterCreation)
{
    memset(&m_nid, 0, sizeof(NOTIFYICONDATA));
#if(WINVER > 0x0500)
    m_nid.cbSize = NOTIFYICONDATA_V2_SIZE;
#else
    m_nid.cbSize = NOTIFYICONDATA_V1_SIZE;
#endif
    if (showAfterCreation) {
        show();
    }

}

NotifyIcon::~NotifyIcon()
{
    if (m_nid.hIcon != 0) {
        setIcon(NULL);
    }
}

const Icon* NotifyIcon::getIcon() const
{
    return m_icon;
}

bool NotifyIcon::isVisible() const
{
    return m_visible;
}

void NotifyIcon::setIcon(Icon* icon)
{
    m_nid.uFlags = NIF_ICON;
    if (icon != 0) {
        m_nid.hIcon = icon->getHICON();
        Shell_NotifyIcon(NIM_MODIFY, &m_nid);
    }
    else {
        hide();
    }
    m_icon = icon;
}

void NotifyIcon::setText(const TCHAR* text)
{
    m_nid.uFlags = NIF_TIP;
    const size_t BUFFER_SIZE_TCHARS = sizeof(m_nid.szTip) / sizeof(TCHAR);
    _tcsncpy_s(m_nid.szTip, BUFFER_SIZE_TCHARS, text, _TRUNCATE);
    Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

void NotifyIcon::showBallon(const TCHAR* message, const TCHAR* caption, DWORD timeoutMillis)
{
    m_nid.uFlags = NIF_INFO;
    _tcsncpy_s(m_nid.szInfo, 255, message, _TRUNCATE);
    _tcsncpy_s(m_nid.szInfoTitle, 63, caption, _TRUNCATE);
    m_nid.dwInfoFlags = NIIF_INFO;
    m_nid.uTimeout = timeoutMillis;
    Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

void NotifyIcon::show()
{
    m_nid.uFlags = NIF_MESSAGE;
    m_nid.hWnd = m_window;
    m_nid.uCallbackMessage = WM_USER + 1;
    Shell_NotifyIcon(NIM_ADD, &m_nid);
    m_visible = true;
}

void NotifyIcon::hide()
{
    m_nid.uFlags = NIF_ICON;
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
    m_nid.hIcon = 0;
    m_visible = false;
}
