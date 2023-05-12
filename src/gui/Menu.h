#pragma once
#include "utils/CommonHeader.h"
#include<crtdbg.h>

class Menu
{
public:
	Menu();
	virtual ~Menu();

	HMENU getMenu();
	void setMenu(HMENU hmenu);
	bool getWindowMenu(HWND hwnd);
	void getSystemMenu(HWND hwnd);
	void create();
	bool loadMenu(LPCTSTR lpMenuName);
	void createPopupMenu();
	bool getSubMenu(int nPos, Menu* menu);
	void termMenu();

	int getMenuItemCount();
	bool appendMenu(StringStorage strVal, UINT uID);
	bool appendSeparator();
	bool appendSubMenu(StringStorage strVal, Menu* menu);

	bool insertMenuItem(UINT uItem, StringStorage strVal, UINT uID);
	bool insertCheckMenuItem(UINT uItem, StringStorage strVal, UINT uID);
	bool insertSeparator(UINT uItem);
	bool insertSubMenu(UINT uItem, StringStorage strVal, Menu* pMenu);

	bool enableMenuItem(UINT uID, UINT uEnable);
	bool checkedMenuItem(UINT uID, bool bEnable);
	bool deleteMenu(UINT uPosition);

	int findMenuItem(UINT uID);
	bool setDefaultItem(UINT uID);
	void operator=(HMENU hmenu);

private:
	bool appendMenu(UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem);
	bool insertMenuItem(UINT uItem, BOOL fByPosition, LPCMENUITEMINFO lpmii);
	bool modifyMenu(UINT uPosition, UINT uFlags, LONG_PTR uIDNewItem, LPCTSTR lpNewItem);
	bool setMenuItem(UINT uItem, BOOL fByPosition, LPMENUITEMINFO lpmii);

protected:
	HMENU m_menu;
	bool m_bCreated;
};

