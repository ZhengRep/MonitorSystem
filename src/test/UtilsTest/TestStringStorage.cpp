#include "Utils/StringParser.h"
#include<wchar.h>
#include<iostream>

int main()
{
	StringParser stringParser;
	const TCHAR str[30] = _T("30");
	unsigned char out[2];
	stringParser.parseByte(str, out);

	return 0;
}