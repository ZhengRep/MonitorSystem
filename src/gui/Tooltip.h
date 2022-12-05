#pragma once
#include "Control.h"

class Tooltip
{
public:
	Tooltip();
	virtual ~Tooltip();
	virtual void showTooltip(Control* control) = 0;

	virtual void setText(const TCHAR* text) = 0;
	virtual void setTitle(const TCHAR* caption) = 0;
	virtual void getText(StringStorage* text) const = 0;
	virtual void getTitle(StringStorage* title) const = 0;

	virtual void setIconType(int iconType) = 0;
	virtual int getIconType() const = 0;
};

