#include "CheckBox.h"

bool CheckBox::isChecked()
{
	return SendMessage(m_hwnd, BM_GETCHECK, NULL, NULL) != 0;
}

void CheckBox::check(bool checked)
{
	SendMessage(m_hwnd, BM_SETCHECK, (checked) ? 1 : 0, NULL);
}
