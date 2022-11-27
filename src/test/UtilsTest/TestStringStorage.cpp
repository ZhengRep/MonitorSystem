#include "Utils/StringParser.h"
#include<wchar.h>
#include<iostream>

int main()
{
	StringParser stringParser;
	const TCHAR str[30] = _T("A0");
	unsigned char out;
	stringParser.parseByteHex(str, &out);

	return 0;
}