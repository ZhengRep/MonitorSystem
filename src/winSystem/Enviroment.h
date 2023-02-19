#pragma once

#include "Utils/StringStorage.h"
#include "log_writer/LogWriter.h"

class Environment
{
public:
	static const int APPLICATION_DATA_SPECIAL_FOLDER = 0x0;
	static const int COMMON_APPLICATION_DATA_SPECIAL_FOLDER = 0x1;

public:
	Environment();
	~Environment();

	static void getErrStr(StringStorage* out);
	static void getErrStr(const TCHAR* specification, StringStorage* out);


};

