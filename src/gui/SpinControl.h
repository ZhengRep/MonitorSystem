#pragma once
#include "Control.h"

class SpinControl: public Control
{
public:
	SpinControl();
	~SpinControl();

	void setBuddy(Control* buddyControl);
	void setRange(short lower, short upper);
	void setRange32(int lower, int upper);
	void setAccel(UINT nSec, UINT nInc);

	void autoAccelerationHandle(LPNMUPDOWN message);
	void enableAutoAcceleration(bool enabled);
	void setAutoAccelerationParams(const std::vector<int>* limitters, const std::vector<int>* deltas, int maxDelta);

protected:
	Control* m_buddy;

	bool m_isAutoAccelerationEnabled;
	std::vector<int> m_limitters;
	std::vector<int> m_deltas;
	int m_maxDelta;
};

