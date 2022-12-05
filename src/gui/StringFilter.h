#pragma once
#include<tchar.h>

class StringFilter
{
public:
	virtual bool isStringCorrect(const TCHAR* string);
};

