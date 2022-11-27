#include "StringParser.h"

#pragma warning(disable: 4996)

bool StringParser::tryParseInt(const TCHAR* str)
{
    return parseInt(str, NULL);
}

bool StringParser::parseInt(const TCHAR* str, int* out)
{
    int value = 0;
    TCHAR c; //null terminater character
    if (_stscanf(str, _T("%d%c"), &value, &c) != 1){
        return false;
    }
    if (out != NULL) {
        *out = value;
    }
    return true;
}

bool StringParser::parseUInt(const TCHAR* str, unsigned int* out)
{
    //check the minus sign manually because _tcsttoul dose not fail on it
    if (!str || str[0] == _T('-')) {
        return false;
    }

    TCHAR* ptr = 0;
    errno = 0;
    unsigned long ulongValue = _tcstoul(str, &ptr, 10);
    if (errno != 0 || ptr == 0 || *ptr != _T('\0')) {
        return false;
    }

    unsigned int uintValue = ulongValue;
    if (uintValue != ulongValue) { //different data type compare
        return false;
    }
    if (out != NULL) {
        *out = uintValue;
    }
    return true;
}

bool StringParser::parseUInt64(const TCHAR* str, UINT64* out)
{
    UINT64 value = 0;
    TCHAR c;
    if (_stscanf(str, _T("%llu%c"), &value, &c) != 1) {
        return false;
    }
    if (out != NULL) {
        *out = value;
    }
    return true;
}

bool StringParser::parseHex(const TCHAR* str, unsigned int* out)
{
    TCHAR c;
    unsigned int val = 0;
    if (_stscanf(str, _T("%x%c"), &val, &c) != 1) {
        return false;
    }
    if (out != NULL) {
        *out = val;
    }
    return true;
}

//byte denotes data width is char
bool StringParser::parseByte(const TCHAR* str, unsigned char* out)
{
    TCHAR c;
    int val = 0;
    if (_stscanf(str, _T("%d%c"), &val, &c) != 1) {
        return false;
    }
    if (out != NULL) {
        *out = (unsigned char)val;
    }
    return true;
}

bool StringParser::parseByteHex(const TCHAR* str, unsigned char* out)
{
    TCHAR c;
    int val = 0;
    if (_stscanf(str, _T("%x%c"), &val, &c) != 1) {
        return false;
    }
    if (out != NULL) {
        *out = (unsigned char)val;
    }
    return true;
}
