#pragma once
#include "StringStorage.h"
#include<map>

class StringTable
{
public:
	static const TCHAR* getString(UINT id);
private:
	StringTable();
	static std::map<UINT, StringStorage> _cache;
};

