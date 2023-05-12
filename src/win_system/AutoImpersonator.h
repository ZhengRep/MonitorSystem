#pragma once

#include "Impersonator.h"
#include "log_writer/LogWriter.h"

class AutoImpersonator
{
public:
	AutoImpersonator(Impersonator* imp, LogWriter* log);
	virtual ~AutoImpersonator();

private:
	Impersonator* m_imp;
	LogWriter* m_log;
};

