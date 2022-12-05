#pragma once
#include "Control.h"

class CheckBox: public Control
{
public:
	bool isChecked();
	void check(bool checked);
};

