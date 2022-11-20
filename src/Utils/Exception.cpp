#include "Exception.h"

Exception::Exception()
{
    m_message.setString(_T(""));
}

Exception::Exception(const TCHAR* format, ...)
{
    va_list vl;
    va_start(vl, format);
    m_message.format(format, vl);
    va_end(vl);
}

Exception::~Exception()
{
}

const TCHAR* Exception::getMessage() const
{
    return m_message.getString();
}
