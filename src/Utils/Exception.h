#pragma once

#pragma warning(disable: 4290)
#include "StringStorage.h"

class Exception
{
public:
	Exception();
	Exception(const TCHAR* format, ...);
	virtual ~Exception();
	const TCHAR* getMessage() const;

protected:
	StringStorage m_message;
};

