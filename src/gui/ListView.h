#pragma once
#include "Utils/CommonHeader.h"
#include "Control.h"
#include<commctrl.h>

typedef struct 
{
	int index;
	LPARAM tag;
}ListViewItem;

class ListView: public Control
{
	ListView();
	void addColumn(int index, const TCHAR* caption, int width, int fmt);
	void addColumn(int index, const TCHAR* caption, int width);

	ListViewItem getItem(int index);

	int getCount();
	void addItem(int index, const TCHAR* caption);
	void addItem(int index, const TCHAR* caption, LPARAM tag);
	void addItem(int index, const TCHAR* caption, LPARAM tag, int imageIndex);
	void removeItem(int i);
	void clear();
	void setSubItemText(int index, int subIndex, const TCHAR* caption);
	void setItemData(int index, LPARAM tag);
	LPARAM getItemData(int index);
	ListViewItem getSelectedItem();
	int getSelectedIndex();
	void selectItem(int index);
	void setFullRowSelectStyle(bool fullRowSelect);
	void allowMultiSelection(bool allow);
	unsigned int getSelectedItemsCount();
	void getSelectedItemsIndexes(int* indexes);

protected:
	void setExStyle(DWORD style);
	DWORD getExStyle();
	void addExStyle(DWORD style);
	void removeExStyle(DWORD style);

	void sort(int columnIndex, PFNLVCOMPARE compareItem);
	void sort();

private:
	bool m_sortAscending;
	int m_sortColumnIndex;
	PFNLVCOMPARE m_compareItem;
};

