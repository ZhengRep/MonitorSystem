#pragma once

#include "Utils/CharDefs.h"

class Logger
{
public:
	virtual void print(int logLevel, const TCHAR* line) = 0;
	virtual bool acceptsLevel(int logLevel) = 0;
};

