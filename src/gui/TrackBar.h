#pragma once
#include "Control.h"

class TrackBar: public Control
{
public:
	TrackBar();
	virtual ~TrackBar();

	void setRange(long min, long max);
	void setPos(long pos);
	long getPos();
};

