#pragma once
#include "Control.h"

class ListBox: public Control
{
public:
	ListBox();
	~ListBox();

	void addString(const TCHAR* str);
	void addString(const TCHAR* str, void* tag);
	
	void getItemText(int index, StringStorage* storage);
	void setItemText(int index, const TCHAR* str);

	void insertString(int index, const TCHAR* str);
	void insertString(int index, const TCHAR* str, LPARAM data);

	void appendString(const TCHAR* str, LPARAM data);
	void setItemData(int index, LPARAM data);
	void removeString(int index);

	int getSelectedIndex();
	int getTopIndex();
	void setTopIndex(int index);
	void setSelectedIndex(int index);
	LPARAM getItemData(int index);
	int getCount();
	void clear();
};

