#pragma once
#include "BaseDialog.h"
#include "Control.h"

class Tab
{
public:
	Tab();
	Tab(BaseDialog* dialog, const TCHAR* caption);

	void setCaption(const TCHAR* caption);
	const TCHAR* getCaption();
	void setDialog(BaseDialog* dialog);
	BaseDialog* getDialog();
	//Method return true if tab has dialog
	bool isOk();
	void setVisible(bool visible);

protected:
	BaseDialog* m_dialog;
	StringStorage m_caption;
};

