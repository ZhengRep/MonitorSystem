#include "TabControl.h"

TabControl::TabControl()
{
}

TabControl::~TabControl()
{
    deleteAllTabs();
}

TabContainer& TabControl::getTabs()
{
    return m_tabContainer;
}

Tab* TabControl::getTab(int index)
{
    if ((index < 0) || (index > m_tabContainer.size() - 1)) {
        return NULL;
    }
    return m_tabContainer.at(index);
}

void TabControl::addTab(BaseDialog* dialog, const TCHAR* caption)
{
    Tab* tab = new Tab(dialog, caption);
    m_tabContainer.push_back(tab);
    TCITEM tcitem = { 0 };
    tcitem.mask = TCIF_TEXT;
    TCHAR fixedCaption[255];
    _tcscpy(&fixedCaption[0], tab->getCaption());
    tcitem.pszText = fixedCaption;
    TabCtrl_InsertItem(getWindow(), m_tabContainer.size() - 1, &tcitem);
}

void TabControl::showTab(int index)
{
    int selectedIndex = getSelectedTabIndex();
    if (selectedIndex >= 0) {
        getTab(selectedIndex)->setVisible(false);
    }
    TabCtrl_SetCurSel(m_hwnd, index);
    getTab(index)->setVisible(true);
}

void TabControl::showTab(const BaseDialog* dialog)
{
    for (size_t i = 0; i < m_tabContainer.size(); i++) {
        if (m_tabContainer.at(i)->getDialog() == dialog) {
            showTab(i);
            return;
        }
    }
}

void TabControl::deleteAllTabs()
{
    for (size_t i = 0; i < m_tabContainer.size(); i++) {
        Tab* tab = m_tabContainer.at(i);
        delete tab;
    }
    m_tabContainer.clear();
    TabCtrl_DeleteAllItems(getWindow());
}

void TabControl::removeTab(int index)
{
    int i = 0; 
    for (TabContainer::iterator it = m_tabContainer.begin(); it != m_tabContainer.end(); it++) {
        if (i == index) {
            delete* it;
            m_tabContainer.erase(it);
            TabCtrl_DeleteItem(m_hwnd, index);
            break;
        }
        i++;
    }
}

int TabControl::getSelectedTabIndex()
{
    int page = TabCtrl_GetCurSel(getWindow());
    return page;
}

void TabControl::adjustRect(RECT* rect)
{
    GetClientRect(m_hwnd, rect);
    TabCtrl_AdjustRect(m_hwnd, FALSE, rect);
}
