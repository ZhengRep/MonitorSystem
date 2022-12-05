#pragma once
#include "Tooltip.h"

class BalloonTip:public Tooltip
{
public:
	BalloonTip(const TCHAR* text, const TCHAR* caption);
	BalloonTip();
	virtual ~BalloonTip();

	void showTooltip(Control* control);

	void setText(const TCHAR* text);
	void setTitle(const TCHAR* caption);

	void getText(StringStorage* text) const;
	void getTitle(StringStorage* title) const;

	void setIconType(int iconType);
	int getIconType() const;

protected:
	StringStorage m_text;
	StringStorage m_title;
};

