#include "ListView.h"

ListView::ListView()
    :m_sortAscending(false)
{
    m_sortColumnIndex = -1;
    m_compareItem = 0;
}

void ListView::addColumn(int index, const TCHAR* caption, int width, int fmt)
{
    LV_COLUMN lvColumn = { 0 };
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvColumn.fmt = fmt;
    lvColumn.cx = width;
    lvColumn.pszText = (TCHAR*)caption;
    ListView_InsertColumn(m_hwnd, index, &lvColumn);
}

void ListView::addColumn(int index, const TCHAR* caption, int width)
{
    addColumn(index, caption, width, LVCFMT_LEFT);
}

ListViewItem ListView::getItem(int index)
{
    ListViewItem item;

    LVITEM lvI;
    TCHAR textBuffer[256];

    lvI.mask = LVIF_TEXT | LVIF_PARAM;
    lvI.pszText = textBuffer;
    lvI.iItem = index;
    lvI.iSubItem = 0;
    lvI.cchTextMax = 256 * sizeof(TCHAR);

    ListView_GetItem(m_hwnd, &lvI);
    item.index = lvI.iItem;
    item.tag = lvI.lParam;

    return item;
}

int ListView::getCount()
{
    return ListView_GetItemCount(m_hwnd);
}

void ListView::addItem(int index, const TCHAR* caption)
{
    addItem(index, caption, NULL);
}

void ListView::addItem(int index, const TCHAR* caption, LPARAM tag)
{
    LVITEM lvI;
    lvI.mask = LVIF_TEXT | LVIF_PARAM;
    lvI.lParam = tag;
    lvI.iItem = index;
    lvI.iSubItem = 0;
    lvI.pszText = (TCHAR*)caption;

    ListView_InsertItem(m_hwnd, &lvI);
}

void ListView::addItem(int index, const TCHAR* caption, LPARAM tag, int imageIndex)
{
    LVITEM lvI;
    lvI.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
    lvI.lParam = tag;
    lvI.iItem = index;
    lvI.iSubItem = 0;
    lvI.iImage = imageIndex;
    lvI.pszText = (TCHAR*)caption;

    ListView_InsertItem(m_hwnd, &lvI);
}

void ListView::removeItem(int i)
{
    ListView_DeleteItem(getWindow(), i);
}

void ListView::clear()
{
    ListView_DeleteAllItems(m_hwnd);
}

void ListView::setSubItemText(int index, int subIndex, const TCHAR* caption)
{
    LVITEM lvI;
    lvI.mask = LVIF_TEXT;
    lvI.iItem = index;
    lvI.iSubItem = subIndex;
    lvI.pszText = (TCHAR*)caption;

    SendMessage(m_hwnd, LVM_SETITEM, 0, (LPARAM)&lvI);
}

void ListView::setItemData(int index, LPARAM tag)
{
    LVITEM lvI;
    lvI.mask = LVIF_PARAM;
    lvI.lParam = tag;
    lvI.iItem = index;
    lvI.iSubItem = 0;

    SendMessage(m_hwnd, LVM_SETITEM, 0, (LPARAM)&lvI);
}

LPARAM ListView::getItemData(int index)
{
    return getItem(index).tag;
}

ListViewItem ListView::getSelectedItem()
{
    ListViewItem item;
    int index = getSelectedIndex();
    if (index == -1) {
        item.index = -1;
        item.tag = NULL;
    }
    else {
        item = getItem(index);
    }
    return item;
}

int ListView::getSelectedIndex()
{
    int iSelect = ListView_GetNextItem(m_hwnd, -1, LVNI_SELECTED);
    return iSelect;
}

void ListView::selectItem(int index)
{
    WPARAM itemIndex = (WPARAM)index;
    ListView_SetItemState(m_hwnd, -1, 0, LVIS_SELECTED);
    SendMessage(m_hwnd, LVM_ENSUREVISIBLE, itemIndex, FALSE);
    ListView_SetItemState(m_hwnd, itemIndex, LVIS_SELECTED, LVIS_SELECTED);
    ListView_SetItemState(m_hwnd, itemIndex, LVIS_FOCUSED, LVIS_FOCUSED);
    setFocus();
}

void ListView::setFullRowSelectStyle(bool fullRowSelect)
{
    if (fullRowSelect) {
        addExStyle(LVS_EX_FULLROWSELECT);
    }
    else {
        removeExStyle(LVS_EX_FULLROWSELECT);
    }
}

void ListView::allowMultiSelection(bool allow)
{
    if (allow) {
        removeStyle(LVS_SINGLESEL);
    }
    else {
        addStyle(LVS_SINGLESEL);
    }
}

unsigned int ListView::getSelectedItemsCount()
{
    return ListView_GetSelectedCount(m_hwnd);
}

unsigned int ListView::getSelectedItemsIndexes(int* indexes)
{
    int i = -1;
    for (unsigned int j = 0; j < getSelectedItemsCount(); j++) {
        i = ListView_GetNextItem(m_hwnd, i, LVNI_SELECTED);
        indexes[j] = i;
    }
}

void ListView::setExStyle(DWORD style)
{
    ::SendMessage(m_hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)style);
}

DWORD ListView::getExStyle()
{
    return ListView_GetExtendedListViewStyle(m_hwnd);
}

void ListView::addExStyle(DWORD style)
{
    DWORD flags = getExStyle();
    flags |= style;
    setExStyle(flags);
}

void ListView::removeExStyle(DWORD style)
{
    DWORD flags = getExStyle();
    flags &= ~style;
    setExStyle(flags);
}

void ListView::sort(int columnIndex, PFNLVCOMPARE compareItem)
{
    int oldSortColumnIndex = m_sortColumnIndex;
    m_sortColumnIndex = columnIndex;

    if (oldSortColumnIndex == m_sortColumnIndex) {
        m_sortAscending = !m_sortAscending;
    }
    else {
        m_sortAscending = true;
    }
    m_compareItem = compareItem;

    //update arrow in header
    HWND hHeader = ListView_GetHeader(m_hwnd);
    if (hHeader != 0) {
        HDITEM hdrItem = { 0 };
        hdrItem.mask = HDI_FORMAT;

        //delete all header icons
        if (Header_GetItem(hHeader, oldSortColumnIndex, &hdrItem)) {
            hdrItem.fmt = hdrItem.fmt & ~HDF_SORTUP & ~HDF_SORTDOWN;
            Header_SetItem(hHeader, oldSortColumnIndex, &hdrItem);
        }

        //add necessary header icon
        if (Header_GetItem(hHeader, m_sortColumnIndex, &hdrItem)) {
            hdrItem.fmt = hdrItem.fmt | (m_sortAscending ? HDF_SORTUP : HDF_SORTDOWN);
            Header_SetItem(hHeader, m_sortColumnIndex, &hdrItem);
        }
    }
    sort();
}

void ListView::sort()
{

}
