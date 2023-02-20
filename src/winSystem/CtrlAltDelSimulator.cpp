#include "CtrlAltDelSimulator.h"

CtrlAltDelSimulator::CtrlAltDelSimulator()
{
	resume();
}

CtrlAltDelSimulator::~CtrlAltDelSimulator()
{
}

void CtrlAltDelSimulator::execute()
{
	//Switch thread desktop to "Winlogon"
	if (DesktopSelector::selectDesktop(&StringStorage(_T("Winlogon")))) {
		HWND hwndCtrlAltDel = FindWindow(_T("SAS window class"), _T("SAS Window"));
		if (hwndCtrlAltDel == NULL) {
			hwndCtrlAltDel = HWND_BROADCAST;
		}
		PostMessage(hwndCtrlAltDel, WM_HOTKEY, 0, MAKELONG(MOD_ALT | MOD_CONTROL, VK_DELETE));
	}
}
