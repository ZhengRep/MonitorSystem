#pragma once

#include "Utils/CommonHeader.h"

class DesktopSelector
{
public:
	static HDESK getInputDesktop();
	static HDESK getDesktop(const StringStorage* name);
	static bool closeDesktop(HDESK hdesk);
	static bool setDesktopToCurrentThread(HDESK newDesktop);
	static bool selectDesktop(const StringStorage* name = 0);
	static bool getCurrentDesktopName(StringStorage* desktopName);
	static bool getThreadDesktopName(StringStorage* desktopName);

private:
	static bool getDesktopName(HDESK desktop, StringStorage* desktopName);
};

