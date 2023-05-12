#pragma once
#include "utils/Exception.h"

class CommonControlsEx
{
public:
	CommonControlsEx();
	virtual ~CommonControlsEx();

	static void init() throw(Exception);
};

