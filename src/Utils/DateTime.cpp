#include "DateTime.h"
#include<crtdbg.h>

DateTime::DateTime()
    :m_timeValue(0)
{
}

DateTime::DateTime(const DateTime& dt)
    :m_timeValue(dt.m_timeValue)
{
}

DateTime::DateTime(UINT64 timeValue)
    :m_timeValue(timeValue)
{
}

DateTime::DateTime(FILETIME ft)
{
    const UINT64 SECS_BETWEEN_EPOCHS = 11644473600;
    const UINT64 SECS_TO_100NS = 10000000;

    UINT64 winTime100ns = ((UINT64)ft.dwHighDateTime << 32) + ft.dwLowDateTime;

    // Time since unix epoch in 100ns intervals.
    UINT64 unixTime100ns = winTime100ns - SECS_BETWEEN_EPOCHS * SECS_TO_100NS;

    m_timeValue = (unixTime100ns / 10000);
}

UINT64 DateTime::getTime() const
{
    return m_timeValue;
}

DateTime DateTime::operator- (const DateTime& other)
{
    return DateTime(getTime() - other.getTime());
}

DateTime DateTime::operator+ (const DateTime& other)
{
    return DateTime(getTime() + other.getTime());
}

void DateTime::toFileTime(LPFILETIME ft) const
{
    UINT64 ll;
    ll = m_timeValue * (10000000 / 1000) + 116444736000000000;
    ft->dwLowDateTime = (DWORD)ll;
    ft->dwHighDateTime = ll >> 32;
}

void DateTime::toUtcSystemTime(LPSYSTEMTIME st) const
{
    FILETIME ft;

    toFileTime(&ft);

    FileTimeToSystemTime(&ft, st);
}

void DateTime::toLocalSystemTime(LPSYSTEMTIME st) const
{
    FILETIME ft, localFt;

    toFileTime(&ft);
    // Convert file time from UTC format to local.
    FileTimeToLocalFileTime(&ft, &localFt);

    FileTimeToSystemTime(&localFt, st);
}

void DateTime::toString(StringStorage* target) const
{
    SYSTEMTIME systemTime;
    toLocalSystemTime(&systemTime);
    const size_t dateStringMaxLength = 255;
    TCHAR dateString[dateStringMaxLength + 1];
    if (GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &systemTime, 0, dateString, dateStringMaxLength)) {

    }

    target->setString(dateString);
    target->appendChar(_T(' '));

    const size_t timeStringMaxLength = 255;
    TCHAR timeString[timeStringMaxLength + 1];
    if (GetTimeFormat(LOCALE_USER_DEFAULT, 0, &systemTime, 0, timeString, timeStringMaxLength) == 0) {

    }

    target->appendString(timeString);
}

DateTime DateTime::now()
{
    SYSTEMTIME st = { 0 };
    GetLocalTime(&st);
    FILETIME ft;
    SystemTimeToFileTime(&st, &ft);
    DateTime dt(ft);
    return dt;
}
