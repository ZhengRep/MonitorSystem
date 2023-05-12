#pragma once

#include "utils/winhdr.h"
#include "SystemException.h"

class Shell
{
public:
	static void runAsAdmin(const TCHAR* pathToFile, const TCHAR* parameters) throw(SystemException);
	static void open(const TCHAR* file, const TCHAR* parameters, const TCHAR* workDirectory) throw(SystemException);

private:
	//Don't allow instanizing of class
	Shell();
};

