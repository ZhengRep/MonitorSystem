#include "UnicodeStringStorage.h"

UnicodeStringStorage::UnicodeStringStorage()
{
    setString(L"");
}

UnicodeStringStorage::UnicodeStringStorage(const WCHAR* string)
{
    setString(string);
}

UnicodeStringStorage::UnicodeStringStorage(const StringStorage* string)
{
    fromStringStorage(string);
}

UnicodeStringStorage::UnicodeStringStorage(const UnicodeStringStorage& stringBuffer)
{
    *this = stringBuffer;
}

UnicodeStringStorage::~UnicodeStringStorage()
{
}

void UnicodeStringStorage::fromStringStorage(const StringStorage* src)
{
#ifndef _UNICODE //multi byte, src is multi byte
    int constrCharCount = src->getLength() + 1;
    m_buffer.resize(constrCharCount);
    MultiByteToWideChar(CP_ACP, 0, src->getString(), constrCharCount, //Size in bytes
        &m_buffer.front(), m_buffer.size());
#else //unicode
    setString(src->getString());
#endif
}

void UnicodeStringStorage::toStringStorage(StringStorage* dst)
{
    dst->setString(_T(""));
#ifndef _UNICODE //multi byte, dst is multi byte
    int symbolCount = m_buffer.size();
    int dstBufferSize = WideCharToMultiByte(CP_ACP, 0, getString(), symbolCount, NULL,
        NULL, NULL, NULL);
    if (dstBufferSize <= 0) {
        return;
    }

    std::vector<char> ansiBuffer(dstBufferSize);
    if (WideCharToMultiByte(CP_ACP, 0, &m_buffer.front(), symbolCount,
        &ansiBuffer.front(), dstBufferSize, NULL, NULL)) {
        dst->setString(&ansiBuffer.front());
    }

#else //unicode
    dst->setString(getString());
#endif
}

void UnicodeStringStorage::setString(const WCHAR* string)
{
    if (string == NULL) {
        string = L"";
    }

    size_t length = wcslen(string);
    m_buffer.resize(length + 1);
    memcpy(&m_buffer.front(), string, getSize());
}

const WCHAR* UnicodeStringStorage::getString() const
{
    return &m_buffer.front();
}

size_t UnicodeStringStorage::getLength() const
{
    return m_buffer.size() - 1;
}

size_t UnicodeStringStorage::getSize() const
{
    return m_buffer.size() * sizeof(WCHAR);
}

bool UnicodeStringStorage::isEmpty() const
{
    return m_buffer.size() == 0;
}
