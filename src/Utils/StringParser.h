#pragma once

#include "CommonHeader.h"
#include "inttypes.h"

class StringParser
{
public:
	static bool tryParseInt(const TCHAR* str);
	static bool parseInt(const TCHAR* str, int* out);
	static bool parseUInt(const TCHAR* str, unsigned int* out);
	static bool parseUInt64(const TCHAR* str, UINT64* out);
	static bool parseHex(const TCHAR* str, unsigned int *out);
	static bool parseByte(const TCHAR* str, unsigned char* out);
	static bool parseByteHex(const TCHAR* str, unsigned char* out);
};

