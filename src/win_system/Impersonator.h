#pragma once

#include "utils/CommonHeader.h"
#include "utils/StringStorage.h"
#include "log_writer/LogWriter.h"
#include "win_system/SystemException.h"

class Impersonator
{
public:
	Impersonator(LogWriter* log);
	virtual ~Impersonator(){};
	virtual void impersonateAsLoggedUser() throw(SystemException);
	virtual void revertToSelf() throw(SystemException);

protected:
	HANDLE m_token;
	HANDLE m_dupToken;

	LogWriter* m_log;
};

