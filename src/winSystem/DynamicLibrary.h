#pragma once

#include "Utils/CommonHeader.h"
#include "Utils/Exception.h"

class DynamicLibrary
{
public:
	DynamicLibrary(const TCHAR* fileName) throw(Exception);
	DynamicLibrary();
	virtual ~DynamicLibrary();

	void init(const TCHAR* fileName);
	FARPROC getProcAddress(const char* procName);

private:
	HMODULE m_module;
};

