#pragma once
#include "Tab.h"
#include "TabContainer.h"
#include "Control.h"

using namespace std;

class TabControl: public Control
{
public:
	TabControl();
	~TabControl();

	TabContainer& getTabs();
	Tab* getTab(int index);
	void addTab(BaseDialog* dialog, const TCHAR* caption);
	void showTab(int index);
	void showTab(const BaseDialog* dialog);
	void deleteAllTabs();
	void removeTab(int index);
	int getSelectedTabIndex();
	void adjustRect(RECT* rect);

protected:
	TabContainer m_tabContainer;
};

