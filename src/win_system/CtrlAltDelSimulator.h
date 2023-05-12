#pragma once

#include "thread/Thread.h"

class CtrlAltDelSimulator: public Thread
{
public:
	CtrlAltDelSimulator();
	virtual ~CtrlAltDelSimulator();

protected:
	virtual void execute();
};

