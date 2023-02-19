#pragma once

#include "Logger.h"
#include "Utils/CharDefs.h"
#include "ProfileLogger.h"

class LogWriter
{
public:
	LogWriter(Logger* logger);
	virtual ~LogWriter();

	//code internal errors
	void interror(const TCHAR* fmt, ...);
	//connection errors and other
	void error(const TCHAR* fmt, ...);
	void warning(const TCHAR* fmt, ...);
	//Write logs with info level which less than the neutral "message" level
	void info(const TCHAR* fmt, ...);
	void detail(const TCHAR* fmt, ...);
	void message(const TCHAR* fmt, ...);
	void debug(const TCHAR* fmt, ...);
	bool isDebug();

	//Add profiler checkpoint
	ProcessorTimes checkPoint(const TCHAR* tag);

protected:
	static const int LOG_INTERR = 0;
	static const int LOG_ERR = 1;
	static const int LOG_WARN = 2;
	static const int LOG_MSG = 3;
	static const int LOG_INFO = 4;
	static const int LOG_DETAIL = 5;
	static const int LOG_DEBUG = 9;

private:
	void vprintLog(int logLevel, const TCHAR* fmt, va_list argList);
	
	Logger* m_logger;
	ProfileLogger* m_profiler;
};

