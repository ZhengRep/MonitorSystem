#include "Utf8StringStorage.h"
#include "Exception.h"
#include "UnicodeStringStorage.h"

Utf8StringStorage::Utf8StringStorage()
{
    StringStorage emptyStr = StringStorage(_T(""));
    fromStringStorage(&emptyStr);
}

Utf8StringStorage::Utf8StringStorage(const std::vector<char>* utf8Buffer)
{
    setString(utf8Buffer);
}

Utf8StringStorage::Utf8StringStorage(const StringStorage* string)
{
    fromStringStorage(string);
}

Utf8StringStorage::Utf8StringStorage(const Utf8StringStorage& string)
{
    *this = string;
}

Utf8StringStorage::~Utf8StringStorage()
{
}

void Utf8StringStorage::setString(const std::vector<char>* utf8Buffer)
{
    m_buffer = *utf8Buffer;
}

const char* Utf8StringStorage::getString() const
{
    return &m_buffer.front();
}

size_t Utf8StringStorage::getSize() const
{
    return m_buffer.size();
}

size_t Utf8StringStorage::getLength() const
{
    return getSize() - 1;
}

void Utf8StringStorage::fromStringStorage(const StringStorage* src)
{
#ifndef _UNICODE
    // 1) From ANSI to UNICODE
    UnicodeStringStorage uniSrc(src);
    const WCHAR* uniString = uniSrc.getString();
    size_t uniLength = uniSrc.getLength();

#else
    const WCHAR* uniString = src->getString();
    size_t uniLength = src->getLength();
#endif

    int constrSrcSize = (int)uniLength + 1;

    // From UNICODE to UTF8
    int dstRequiredSize = WideCharToMultiByte(CP_UTF8, 0, uniString,
        constrSrcSize, NULL, 0,
        0, 0);

    if (dstRequiredSize == 0) {
        throw Exception(_T("Cannot convert a string to the UTF8 format"));
    }

    m_buffer.resize(dstRequiredSize);
    WideCharToMultiByte(CP_UTF8, 0, uniString, constrSrcSize,
        &m_buffer.front(), dstRequiredSize, 0, 0);
}

void Utf8StringStorage::toStringStorage(StringStorage* dst)
{
    // 1) From UTF8 to UNICODE
    int constrSize = (int)getSize();
    int dstReqSizeInSym = MultiByteToWideChar(CP_UTF8, 0, &m_buffer.front(),
        constrSize, 0, 0);
    if (dstReqSizeInSym == 0) {
        throw Exception(_T("Cannot convert a string from the UTF8 format"));
    }
    std::vector<WCHAR> uniBuff(dstReqSizeInSym + 1);
    MultiByteToWideChar(CP_UTF8, 0, &m_buffer.front(),
        constrSize, &uniBuff.front(), dstReqSizeInSym);
    // Add termination symbol to the unicode buffer because the source string
    // may be without it.
    uniBuff[dstReqSizeInSym] = L'\0';

    // 2) From UNICODE to StringStorage
    UnicodeStringStorage uniString(&uniBuff.front());
    uniString.toStringStorage(dst);
}
