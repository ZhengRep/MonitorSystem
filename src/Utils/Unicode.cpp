#include "Unicode.h"

unsigned short Unicode::SIGNATURE = 0xFEFF;

bool Unicode::isEnabled()
{
#ifdef _UNICODE
    return true;
#endif
    return false;
}
/*
#ifdef _UNICODE //affect character set the C runtime header files
#ifndef UNICODE //affect character set the windows header file
#define UNICODE
#endif
#endif
*/
