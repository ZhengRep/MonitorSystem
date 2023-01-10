#pragma once
#include "Control.h"

class ProgressBar :public Control
{
public:
	ProgressBar();
	~ProgressBar();

	void setRange(WORD min, WORD max);
	void setPos(WORD pos);
};

