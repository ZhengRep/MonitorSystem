#include "ComboBox.h"
#include<windowsx.h>

ComboBox::ComboBox()
{
}

ComboBox::~ComboBox()
{
}

int ComboBox::addItem(const TCHAR* text)
{
    return ComboBox_AddString(m_hwnd, text); //macro to send CB_ADDSTRING message
}

int ComboBox::addItem(const TCHAR* text, void* tag)
{
    int index = addItem(text);
    setItemData(index, tag);
    return index;
}

void ComboBox::insertItem(int index, const TCHAR* text)
{
    ComboBox_InsertString(m_hwnd, index, text);
}

void ComboBox::insertItem(int index, const TCHAR* text, void* tag)
{
    insertItem(index, text);
    setItemData(index, tag);
}

int ComboBox::getItemCount() const
{
    return ComboBox_GetCount(m_hwnd);
}

void ComboBox::setItemData(int index, void* tag) const
{
    ComboBox_SetItemData(m_hwnd, index, (LPARAM)tag);
}

void* ComboBox::getItemData(int index) const
{
    return (void*)ComboBox_GetItemData(m_hwnd, index);
}

void ComboBox::getItemText(int index, StringStorage* storage) const
{
    size_t length = ComboBox_GetLBTextLen(m_hwnd, index);
    std::vector<TCHAR> buf(length + 1);
    ComboBox_GetLBText(m_hwnd, index, &buf.front());
    storage->setString(&buf.front());
}

int ComboBox::getSelectedItemIndex()
{
    return ComboBox_GetCurSel(m_hwnd);
}

void ComboBox::setSelectedItem(int index)
{
    ComboBox_SetCurSel(m_hwnd, index);
}

void ComboBox::deleteItem(int index)
{
    ComboBox_DeleteString(m_hwnd, index);
}

void ComboBox::removeAllItems()
{
    ComboBox_ResetContent(m_hwnd);
}
