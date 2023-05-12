#pragma once
#include "utils/CommonHeader.h"
#include<Uxtheme.h>
#include<vssym32.h>

//Define type for api functions

typedef HRESULT (__stdcall* PFNCLOSETHEMEDATA)(HTHEME hTheme);
typedef HRESULT(__stdcall* PFNDRAWTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc,
	int iPartId, int iStateId, const RECT* pRect, const RECT* pClipTect);

typedef HRESULT (__stdcall* PFNOPENTHEMEDATA)(HWND hwnd, LPCWSTR pszClassList);
typedef HRESULT(__stdcall* PFNDRAWTHEMETEXT)(HTHEME hTheme, HDC hdc, int iPartId,
    int iStateId, LPCWSTR pszText,
    int iCharCount, DWORD dwTextFlags,
    DWORD dwTextFlags2, const RECT* pRect);

typedef HRESULT(__stdcall* PFNGETTHEMEBACKGROUNDCONTENTRECT)(HTHEME hTheme, HDC hdc,
    int iPartId, int iStateId,
    const RECT* pBoundingRect,
    RECT* pContentRect);

class ThemeLib
{
public:
    ThemeLib();
    ~ThemeLib();

    static bool initialize();
    static void deinitialize();

    static bool isLoaded();

    static HRESULT CloseThemeData(HTHEME hTheme);

    static HRESULT DrawThemeBackground(HTHEME hTheme, HDC hdc,
        int iPartId, int iStateId,
        const RECT* pRect,
        const RECT* pClipRect);

    static HTHEME OpenThemeData(HWND hwnd, LPCWSTR pszClassList);

    static HRESULT DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId,
        int iStateId, LPCWSTR pszText,
        int iCharCount, DWORD dwTextFlags,
        DWORD dwTextFlags2, const RECT* pRect);

    static HRESULT GetThemeBackgroundContentRect(HTHEME hTheme, HDC hdc,
        int iPartId, int iStateId,
        const RECT* pBoundingRect,
        RECT* pContentRect);

private:
    static bool s_isThemeLibLoaded;
    static HMODULE s_lib;

    static PFNOPENTHEMEDATA s_OpenThemeData;
    static PFNDRAWTHEMEBACKGROUND s_DrawThemeBackground;
    static PFNCLOSETHEMEDATA s_CloseThemeData;
    static PFNDRAWTHEMETEXT s_DrawThemeText;
    static PFNGETTHEMEBACKGROUNDCONTENTRECT s_GetThemeBackgroundContentRect;
};

