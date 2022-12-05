#include "CommonControlsEx.h"

CommonControlsEx::CommonControlsEx()
{
}

CommonControlsEx::~CommonControlsEx()
{
}

void CommonControlsEx::init() throw(Exception)
{
	INITCOMMONCONTROLSEX iccsex = { 0 };

	iccsex.dwICC = ICC_LISTVIEW_CLASSES | ICC_NATIVEFNTCTL_CLASS |
		ICC_PAGESCROLLER_CLASS | ICC_PROGRESS_CLASS | ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES |
		ICC_UPDOWN_CLASS | ICC_USEREX_CLASSES;
	iccsex.dwSize = sizeof(INITCOMMONCONTROLSEX);

	if (InitCommonControlsEx(&iccsex) == FALSE) {
		throw Exception(_T("Cannot initialize common controls ex"));
	}
}
