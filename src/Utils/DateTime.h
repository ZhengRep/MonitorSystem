#pragma once

#include "inttypes.h"
#include "StringStorage.h"
#include "winhdr.h"

class DateTime
{
public:
	DateTime();
	DateTime(const DateTime& dt);

	DateTime(UINT64 timeValue);
	DateTime(FILETIME ft);
	UINT64 getTime() const;

	DateTime operator-(const DateTime& dt);
	DateTime operator+(const DateTime& dt);

	void toFileTime(LPFILETIME ft) const;
	void toUtcSystemTime(LPSYSTEMTIME st) const;
	void toLocalSystemTime(LPSYSTEMTIME st) const;
	void toString(StringStorage* out) const;
	static DateTime now();

protected:
	UINT64 m_timeValue;
};

