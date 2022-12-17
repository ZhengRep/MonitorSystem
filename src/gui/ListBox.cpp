#include "ListBox.h"

ListBox::ListBox()
{
}

ListBox::~ListBox()
{
}

void ListBox::addString(const TCHAR* str)
{
    addString(str, 0);
}

void ListBox::addString(const TCHAR* str, void* tag)
{
    LRESULT lresult = SendMessage(m_hwnd, LB_ADDSTRING, 0, (LPARAM)str);
    int i = (int)lresult;
    _ASSERT(i == lresult);
    setItemData(i, (LPARAM)tag);
}

void ListBox::getItemText(int index, StringStorage* storage)
{
    size_t length = SendMessage(m_hwnd, LB_GETTEXTLEN, index, NULL);
    _ASSERT(length <= 65536);
    std::vector<TCHAR> buffer(length + 1);
    SendMessage(m_hwnd, LB_GETTEXT, index, (LPARAM)&buffer.front());
    storage->setString(&buffer.front());
}

void ListBox::setItemText(int index, const TCHAR* str)
{
    int si = getSelectedIndex();
    int top = getTopIndex();
    LRESULT lresult = SendMessage(m_hwnd, LB_GETTOPINDEX, NULL, NULL);
    UINT topIndex = (UINT)lresult;
    _ASSERT(lresult == topIndex);
    LPARAM data = getItemData(index);
    removeString(index);
    insertString(index, str, data);
    if (si == index) {
        setSelectedIndex(si);
    }
    setTopIndex(top);
}

void ListBox::insertString(int index, const TCHAR* str)
{
    SendMessage(m_hwnd, LB_INSERTSTRING, index, (LPARAM)str);
}

void ListBox::insertString(int index, const TCHAR* str, LPARAM data)
{
    LRESULT lresult = SendMessage(m_hwnd, LB_INSERTSTRING, index, (LPARAM)str);
    int i = (int)lresult;
    _ASSERT(i == lresult);
    setItemData(i, data);
}

void ListBox::appendString(const TCHAR* str, LPARAM data)
{
    LRESULT lresult = SendMessage(m_hwnd, LB_ADDSTRING, 0, (LPARAM)str);
    int index = (int)lresult;
    _ASSERT(index == lresult);
    setItemData(index, data);
}

void ListBox::setItemData(int index, LPARAM data)
{
    SendMessage(m_hwnd, LB_SETITEMDATA, index, data);
}

void ListBox::removeString(int index)
{
    int top = getTopIndex();
    SendMessage(m_hwnd, LB_DELETESTRING, index, NULL);
    setTopIndex(top);
}

int ListBox::getSelectedIndex()
{
    LRESULT lresult = SendMessage(m_hwnd, LB_GETCURSEL, NULL, NULL);
    int index = (int)lresult;
    _ASSERT(index == lresult);
    return (index == LB_ERR) ? -1 : index;
}

int ListBox::getTopIndex()
{
    return SendMessage(m_hwnd, LB_GETTOPINDEX, NULL, NULL);
}

void ListBox::setTopIndex(int index)
{
    SendMessage(m_hwnd, LB_SETTOPINDEX, index, NULL);
}

void ListBox::setSelectedIndex(int index)
{
    SendMessage(m_hwnd, LB_SETCURSEL, index, NULL);
}

LPARAM ListBox::getItemData(int index)
{
    return SendMessage(m_hwnd, LB_GETITEMDATA, index, NULL);
}

int ListBox::getCount()
{
    LRESULT lresult = SendMessage(m_hwnd, LB_GETCOUNT, NULL, NULL);
    int result = (int)lresult;
    _ASSERT(result == lresult);
    return result;
}

void ListBox::clear()
{
    SendMessage(m_hwnd, LB_RESETCONTENT, NULL, NULL);
}
