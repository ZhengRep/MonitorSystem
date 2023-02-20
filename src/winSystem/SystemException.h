#pragma once

#include "Utils/Exception.h"

class SystemException: public Exception
{
public:
	SystemException();
	virtual ~SystemException();
	SystemException(int errcode);
	SystemException(const TCHAR* userMessage);
	SystemException(const TCHAR* userMessage, int errCode);
	int getErrorCode() const;

	const TCHAR* getSystemErrorDescription() const;

private:
	void createMessage(const TCHAR* userMessage, int errCode);
	StringStorage m_systemMessage;
	int m_errcode;
};

