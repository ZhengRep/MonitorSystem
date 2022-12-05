#pragma once
#include "Control.h"

class ComboBox : public Control
{
public:
	ComboBox();
	~ComboBox();

	//Adds new item to the end of combo box items list
	int addItem(const TCHAR* text);
	int addItem(const TCHAR* text, void* tag);

	//Inserts new item to the specified position
	void insertItem(int index, const TCHAR* text);
	void insertItem(int index, const TCHAR* text, void* tag);

	int getItemCount() const;
	
	void setItemData(int index, void *tag) const;
	void* getItemData(int index) const;

	virtual void getItemText(int index, StringStorage* storage) const;

	int getSelectedItemIndex();
	void setSelectedItem(int index);
	void deleteItem(int index);
	void removeAllItems();

};

