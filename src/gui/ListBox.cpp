#include "ListBox.h"

ListBox::ListBox()
{
}

ListBox::~ListBox()
{
}

void ListBox::addString(const TCHAR* str)
{
}

void ListBox::addString(const TCHAR* str, void* tag)
{
}

void ListBox::getItemText(int index, StringStorage* storage)
{
}

void ListBox::setItemText(int index, const TCHAR* str)
{
}

void ListBox::insertString(int index, const TCHAR* str)
{
}

void ListBox::insertString(int index, const TCHAR* str, LPARAM data)
{
}

void ListBox::appendString(const TCHAR* str, LPARAM data)
{
}

void ListBox::setItemData(int index, LPARAM data)
{
}

void ListBox::removeString(int index)
{
}

int ListBox::getSelectedIndex()
{
    return 0;
}

int ListBox::getTopIndex()
{
    return 0;
}

void ListBox::setTopIndex(int index)
{
}

void ListBox::setSelectedIndex(int index)
{
}

LPARAM ListBox::getItemData(int index)
{
    return LPARAM();
}

int ListBox::getCount()
{
    return 0;
}

void ListBox::clear()
{
}
