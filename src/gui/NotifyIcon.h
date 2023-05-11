#pragma once
#include "NotifyIconWindow.h"
#include "drawing/Icon.h"

#include<shellapi.h>

class NotifyIcon:public NotifyIconWindow
{
public:
	NotifyIcon(bool showAfterCreation = true);
	virtual ~NotifyIcon();

	const Icon* getIcon() const;
	bool isVisible() const;

	void setIcon(Icon* icon);
	void setText(const TCHAR* text);
	void showBallon(const TCHAR* message, const TCHAR* caption, DWORD timeoutMillis);
	void show();
	void hide();

protected:
	NOTIFYICONDATA m_nid;
	Icon* m_icon;
	bool m_visible;
};

