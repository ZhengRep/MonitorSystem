#include "ResourceLoader.h"
#include "UnicodeStringStorage.h"

ResourceLoader::ResourceLoader(HINSTANCE appInst)
    :m_appInstance(appInst)
{
}

ResourceLoader::~ResourceLoader()
{
}

HICON ResourceLoader::loadStandartIcon(const TCHAR* iconName)
{
    return LoadIcon(NULL, iconName);
}

HICON ResourceLoader::loadIcon(const TCHAR* iconName)
{
    return LoadIcon(m_appInstance, iconName);
}

bool ResourceLoader::loadString(UINT id, StringStorage* string)
{
    _ASSERT(string != 0);
    string->setString(_T("(Undef)"));

    int resId = (id / 16) + 1;
    HRSRC  resHnd = FindResource(m_appInstance,  MAKEINTRESOURCE(resId), RT_STRING);

    if (resHnd) {
        HGLOBAL hGlobal = LoadResource(m_appInstance, resHnd);
        LPVOID lockRes = LockResource(hGlobal);

        WCHAR* lpStr = reinterpret_cast<WCHAR*>(lockRes);
        for (UINT i = 0; i < (id % 16); i++) {
            lpStr += 1 + static_cast<UINT16> (lpStr[0]);
        }
        UINT16 strLen = static_cast<UINT16>(lpStr[0]);

        std::vector<WCHAR> strBuff;
        strBuff.resize(strLen + 1);
        memcpy(&strBuff.front(), lpStr + 1, strLen * sizeof(WCHAR));
        strBuff[strLen] = L'\0';

        UnlockResource(lockRes);
        FreeResource(hGlobal);

        UnicodeStringStorage unicodeString;
        unicodeString.setString(&strBuff.front());
        unicodeString.toStringStorage(string);
    }
    return true;

}

HACCEL ResourceLoader::loadAccelerator(UINT id)
{
    return LoadAccelerators(m_appInstance, MAKEINTRESOURCE(id));
}

HCURSOR ResourceLoader::loadStandardCursor(const TCHAR* id)
{
    return LoadCursor(0, id);
}

HCURSOR ResourceLoader::loadCursor(UINT id)
{
    return LoadCursor(m_appInstance, MAKEINTRESOURCE(id));
}
