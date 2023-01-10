#include "Menu.h"

Menu::Menu()
    :m_menu(0),
    m_bCreated(false)
{
}

Menu::~Menu()
{
    if (m_menu && m_bCreated) {
        DestroyMenu(m_menu);
    }
    m_menu = 0;
    m_bCreated = false;
}

HMENU Menu::getMenu()
{
    return m_menu;
}

//must set once
void Menu::setMenu(HMENU hmenu)
{
    _ASSERT(hmenu == 0);
    m_menu = hmenu;
}

bool Menu::getWindowMenu(HWND hwnd)
{
    _ASSERT(m_menu == 0);

    m_menu = GetMenu(hwnd);
    return m_menu == 0 ? false : true;
}

void Menu::getSystemMenu(HWND hwnd)
{
    _ASSERT(m_menu == 0);
    m_menu = GetSystemMenu(hwnd, FALSE);
}

void Menu::create()
{
    _ASSERT(m_menu == 0);
    m_menu = CreateMenu();
    m_bCreated = true;
}

bool Menu::loadMenu(LPCTSTR lpMenuName)
{
    _ASSERT(m_menu == 0);

    m_menu = LoadMenu(GetModuleHandle(0), lpMenuName);
    m_bCreated = true;
    return m_menu ? true : false;
}

void Menu::createPopupMenu()
{
    _ASSERT(m_menu == 0);

    m_menu = CreatePopupMenu();
    m_bCreated = true;
}

bool Menu::getSubMenu(int nPos, Menu* menu)
{
    HMENU tmenu;
    _ASSERT(m_menu && menu);
    tmenu = GetSubMenu(m_menu, nPos);
    if (tmenu) {
        menu->setMenu(tmenu);
    }
    return tmenu ? true : false;
}

void Menu::termMenu()
{
    if (m_menu && m_bCreated) {
        DestroyMenu(m_menu);
    }
    m_menu = 0;
    m_bCreated = false;
}

int Menu::getMenuItemCount()
{
    _ASSERT(m_menu);

    return GetMenuItemCount(m_menu);
}

bool Menu::appendMenu(StringStorage strVal, UINT uID)
{
    return appendMenu(MF_STRING, uID, strVal.getString());
}

bool Menu::appendSeparator()
{
    return appendMenu(MF_MENUBREAK, 0, 0);
}

bool Menu::appendSubMenu(StringStorage strVal, Menu* pMenu)
{
    return appendMenu(MF_POPUP, (UINT_PTR)pMenu->getMenu(), strVal.getString());
}

bool Menu::insertMenuItem(UINT uItem, StringStorage strVal, UINT uID)
{
    MENUITEMINFO mii;

    ZeroMemory(&mii, sizeof(MENUITEMINFO));
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING | MIIM_ID;
    mii.dwTypeData = (LPTSTR)strVal.getString();
    mii.fState = MFS_DEFAULT;
    mii.wID = uID;

    return insertMenuItem(uItem, TRUE, &mii);
}

bool Menu::insertCheckMenuItem(UINT uItem, StringStorage strVal, UINT uID)
{
    MENUITEMINFO mii;

    ZeroMemory(&mii, sizeof(MENUITEMINFO));
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING | MIIM_ID | MIIM_STATE;
    mii.dwTypeData = (LPTSTR)strVal.getString();
    mii.fState = MFS_UNCHECKED;
    mii.wID = uID;
    return insertMenuItem(uItem, TRUE, (LPMENUITEMINFO)&mii);
}

bool Menu::insertSeparator(UINT uItem)
{
    MENUITEMINFO mii;

    ZeroMemory(&mii, sizeof(MENUITEMINFO));
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fType = MFT_SEPARATOR;
    return insertMenuItem(uItem, TRUE, (LPMENUITEMINFO)&mii);
}

bool Menu::insertSubMenu(UINT uItem, StringStorage strVal, Menu* pMenu)
{
    MENUITEMINFO mii;

    ZeroMemory(&mii, sizeof(MENUITEMINFO));
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fType = MFT_SEPARATOR;
    return insertMenuItem(uItem, TRUE, (LPMENUITEMINFO)&mii);
}

bool Menu::enableMenuItem(UINT uID, UINT uEnable)
{
    _ASSERT(m_menu != 0);

    return !!EnableMenuItem(m_menu, uID, uEnable);
}

bool Menu::checkedMenuItem(UINT uID, bool bEnable)
{
    _ASSERT(m_menu);

    int pos = findMenuItem(uID);
    if (pos == -1) {
        return false;
    }
    UINT command = bEnable ? MF_CHECKED : MF_UNCHECKED;
    DWORD  res = CheckMenuItem(m_menu, pos, MF_BYPOSITION | command);
    return res == -1 ? false : true;
}

bool Menu::deleteMenu(UINT uPosition)
{
    _ASSERT(m_menu);

    return !!DeleteMenu(m_menu, uPosition, MF_BYPOSITION);
}

int Menu::findMenuItem(UINT uID)
{
    _ASSERT(m_menu != 0);

    for (int i = 0; i < getMenuItemCount(); i++) {
        if (GetMenuItemID(m_menu, i) == uID) {
            return i;
        }
    }
    return -1;
}

bool Menu::setDefaultItem(UINT uID)
{
    _ASSERT(m_menu != 0);

    return !!SetMenuDefaultItem(m_menu, uID, FALSE);
}

void Menu::operator=(HMENU hmenu)
{
    setMenu(hmenu);
}

bool Menu::appendMenu(UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem)
{
    _ASSERT(m_menu);

    return !!AppendMenu(m_menu, uFlags, uIDNewItem, lpNewItem);
}

bool Menu::insertMenuItem(UINT uItem, BOOL fByPosition, LPCMENUITEMINFO lpmii)
{
    _ASSERT(m_menu != 0);

    return !!InsertMenuItem(m_menu, uItem, fByPosition, lpmii);
}

bool Menu::modifyMenu(UINT uPosition, UINT uFlags, LONG_PTR uIDNewItem, LPCTSTR lpNewItem)
{
    _ASSERT(m_menu != 0);

    return !!ModifyMenu(m_menu, uPosition, uFlags, uIDNewItem, lpNewItem);
}

bool Menu::setMenuItem(UINT uItem, BOOL fByPosition, LPMENUITEMINFO lpmii)
{
    _ASSERT(m_menu != 0);

    return !!SetMenuItemInfo(m_menu, uItem, fByPosition, lpmii);
}
