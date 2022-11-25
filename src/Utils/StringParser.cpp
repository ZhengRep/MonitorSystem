#include "StringParser.h"

bool StringParser::tryParseInt(const TCHAR* str)
{
    
}

bool StringParser::parseInt(const TCHAR* str, int* out)
{
    int value = 0;
    TCHAR c;
    if(_stscanf(str, _T("%d%c")))
}

bool StringParser::parseUInt(const TCHAR* str, unsigned int* out)
{
    return false;
}

bool StringParser::parseUInt64(const TCHAR* str, UINT64* out)
{
    return false;
}

bool StringParser::parsehex(const TCHAR* str, unsigned int* out)
{
    return false;
}

bool StringParser::parseByte(const TCHAR* str, unsigned char* out)
{
    return false;
}

bool StringParser::parseByteHex(const TCHAR* str, unsigned char* out)
{
    return false;
}
