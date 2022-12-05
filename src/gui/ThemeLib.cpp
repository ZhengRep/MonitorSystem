#include "ThemeLib.h"

PFNOPENTHEMEDATA ThemeLib::s_OpenThemeData = NULL;
PFNDRAWTHEMEBACKGROUND ThemeLib::s_DrawThemeBackground = NULL;
PFNCLOSETHEMEDATA ThemeLib::s_CloseThemeData = NULL;
PFNDRAWTHEMETEXT ThemeLib::s_DrawThemeText = NULL;
PFNGETTHEMEBACKGROUNDCONTENTRECT ThemeLib::s_GetThemeBackgroundContentRect = NULL;

bool ThemeLib::s_isThemeLibLoaded = false;

HMODULE ThemeLib::s_lib = 0;

ThemeLib::ThemeLib()
{
}

ThemeLib::~ThemeLib()
{
}

bool ThemeLib::initialize()
{
    s_lib = LoadLibrary(_T("UXTHEME.DLL"));
    if (s_lib) {

        s_OpenThemeData = (PFNOPENTHEMEDATA)GetProcAddress(s_lib, "OpenThemeData");
        s_DrawThemeBackground = (PFNDRAWTHEMEBACKGROUND)GetProcAddress(s_lib, "DrawThemeBackground");
        s_CloseThemeData = (PFNCLOSETHEMEDATA)GetProcAddress(s_lib, "CloseThemeData");
        s_DrawThemeText = (PFNDRAWTHEMETEXT)GetProcAddress(s_lib, "DrawThemeText");
        s_GetThemeBackgroundContentRect = (PFNGETTHEMEBACKGROUNDCONTENTRECT)GetProcAddress(s_lib, "GetThemeBackgroundContentRect");

        if (s_OpenThemeData && s_DrawThemeBackground && s_CloseThemeData && s_DrawThemeText && s_GetThemeBackgroundContentRect) {
            s_isThemeLibLoaded = true;
        }
    }
    return s_isThemeLibLoaded;
}

void ThemeLib::deinitialize()
{
    if (s_lib) {
        FreeLibrary(s_lib);
    }

    s_isThemeLibLoaded = false;

    s_OpenThemeData = NULL;
    s_DrawThemeBackground = NULL;
    s_CloseThemeData = NULL;
    s_DrawThemeText = NULL;
    s_GetThemeBackgroundContentRect = NULL;
}

bool ThemeLib::isLoaded()
{
    return s_isThemeLibLoaded;
}

HRESULT ThemeLib::CloseThemeData(HTHEME hTheme)
{
    return s_CloseThemeData(hTheme);
}

HRESULT ThemeLib::DrawThemeBackground(HTHEME hTheme, HDC hdc,
    int iPartId, int iStateId,
    const RECT* pRect,
    const RECT* pClipRect)
{
    return s_DrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

HTHEME ThemeLib::OpenThemeData(HWND hwnd, LPCWSTR pszClassList)
{
    return (HTHEME)s_OpenThemeData(hwnd, pszClassList);
}

HRESULT ThemeLib::DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId,
    int iStateId, LPCWSTR pszText,
    int iCharCount, DWORD dwTextFlags,
    DWORD dwTextFlags2, const RECT* pRect)
{
    return s_DrawThemeText(hTheme, hdc, iPartId, iStateId, pszText,
        iCharCount, dwTextFlags, dwTextFlags2, pRect);
}

HRESULT ThemeLib::GetThemeBackgroundContentRect(HTHEME hTheme, HDC hdc,
    int iPartId, int iStateId,
    const RECT* pBoundingRect,
    RECT* pContentRect)
{
    return s_GetThemeBackgroundContentRect(hTheme, hdc, iPartId, iStateId,
        pBoundingRect, pContentRect);
}
