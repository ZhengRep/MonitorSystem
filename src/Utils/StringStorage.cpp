#include "StringStorage.h"
#include "CommonHeader.h"
#include "Exception.h"
#include<algorithm>
#include<stdlib.h>
#include<crtdbg.h>

StringStorage::StringStorage()
{
    setString(_T(""));
}

StringStorage::StringStorage(const TCHAR* string)
{
    setString(string);
}

StringStorage::StringStorage(const StringStorage& stringBuffer)
{
    *this = stringBuffer;
}

StringStorage::~StringStorage()
{
}

void StringStorage::setString(const TCHAR* string)
{
    if (string == 0) {
        string = _T("");
    }
    size_t length = _tcslen(string);
    resizeBuffer(length + 1);
    memcpy(&m_buffer.front(), string, getSize());
}

const TCHAR* StringStorage::getString() const
{
    return &m_buffer.front();
}

size_t StringStorage::getLength() const
{
    return m_buffer.size() - 1;
}

size_t StringStorage::getSize() const
{
    return m_buffer.size() * sizeof(TCHAR);
}

bool StringStorage::isEmpty() const
{
    return getLength() == 0;
}

bool StringStorage::endsWith(TCHAR postfix) const
{
    if (isEmpty()) {
        return false;
    }
    TCHAR lastCharacter = m_buffer[getLength() - 1];
    return (lastCharacter == postfix);
}

bool StringStorage::beginsWith(TCHAR prefix) const
{
    if (isEmpty()) {
        return false;
    }
    TCHAR firstCharacter = m_buffer[0];
    return (firstCharacter == prefix);
}

void StringStorage::getSubstring(StringStorage* substr, size_t startIndex, size_t endIndex) const
{
    endIndex++;
    startIndex = min(startIndex, getLength()); //this is macro
    endIndex = min(endIndex, getLength());
    if (endIndex < startIndex) {
        endIndex = startIndex;
    }
    size_t length = endIndex - startIndex;
    _ASSERT(length <= getLength());
    std::vector<TCHAR> autoBuffer(length + 1);
    TCHAR* buffer = &autoBuffer.front();
    memcpy(buffer, &m_buffer[startIndex], length * sizeof(TCHAR));
    buffer[length] = '\0';
    substr->setString(buffer);
}

void StringStorage::appendChar(TCHAR c)
{
    TCHAR string[2] = { c, _T('\0') };
    appendString(string);
}

void StringStorage::appendString(const TCHAR* string)
{
    if (string == 0) {
        return;
    }
    StringStorage src(string);
    BufferType::iterator to = m_buffer.begin() + getLength();
    BufferType::iterator fromFirst = src.m_buffer.begin();
    BufferType::iterator fromLast = src.m_buffer.begin() + src.getLength();
    m_buffer.insert(to, fromFirst, fromLast);
}

void StringStorage::quoteSelf()
{
    StringStorage quoteString;
    quoteString.format(_T("\"%s\""), getString());
    setString(quoteString.getString());
}

bool StringStorage::isEqualTo(const TCHAR* other) const
{
    if (other == 0 && getString() == 0) {
        return true;
    }
    if (other == 0 && getString() != 0) {
        return false;
    }
    if (getString() == 0 && other != 0) {
        return false;
    }
    if (getString() != 0 && other != 0) {
        return _tcscmp(getString(), other) == 0;
    }
}

bool StringStorage::isEqualTo(const StringStorage* other) const
{
    return isEqualTo(other->getString());
}

bool StringStorage::split(const TCHAR* delimiters, StringStorage* stringArray, size_t* arrayLength) const
{
    //if this is empty string
    if (this->getLength() == 0) {
        *arrayLength = 0;
        return true;
    }

    StringStorage copy(getString());
    size_t chunksCount = 0;
    size_t index = -1;
    do {
        StringStorage chunk(_T(""));
        index = copy.findOneOf(delimiters);

        if (index == (size_t)-1 && chunksCount == 0) {
            if (arrayLength != NULL) {
                if (stringArray != NULL) {
                    if (*arrayLength < 1) {
                        return false;
                    }
                }
                *arrayLength = 1;
            }
            else {
                return false;
            }

            if (stringArray != NULL) {
                stringArray->setString(getString());
            }

            return true;
        }

        if (index == -1) {
            chunk = copy;
        }
        else {
            copy.getSubstring(&chunk, 0, index - 1);
        }

        copy.getSubstring(&copy, index + 1, copy.getLength() - 1);

        if (stringArray != NULL && chunksCount >= *arrayLength) {
            return false;
        }
        else if(stringArray != NULL) {
            stringArray[chunksCount] = chunk;
        }

        chunksCount++;
    } while (index != -1);

    *arrayLength = chunksCount;
    return true;
}

TCHAR* StringStorage::find(const TCHAR* subStr)
{
    return _tcsstr(&m_buffer.front(), subStr);
}

size_t StringStorage::findChar(const TCHAR c)
{
    size_t length = getLength();
    for (size_t i = 0; i < length; i++) {
        if (m_buffer[i] == c) {
            return i;
        }
    }
    return (size_t)-1;
}

//returns first position of any character that string argument have
size_t StringStorage::findOneOf(const TCHAR* string)
{
    size_t length = getLength();
    size_t argLength = _tcslen(string);
    for (size_t i = 0; i < length; i++) {
        for (size_t j = 0; j < argLength; j++) {
            if (m_buffer[i] == string[j]) {
                return i;
            }
        }
    }
    return (size_t) - 1;
}

size_t StringStorage::findLast(const TCHAR c)
{
    for (size_t i = getLength() - 1; i + 1 != 0; i--) {
        if (m_buffer[i] == c) {
            return i;
        }
    }
    return (size_t)-1;
}

void StringStorage::removeChars(const TCHAR badCharacters[], size_t count)
{
    size_t j = 0;
    size_t length = getLength();

    for (size_t i = 0; i < length; i++) {
        TCHAR each = m_buffer[i];
        bool badCharacter = false;
        for (size_t k = 0; k < count; k++) {
            if (each == badCharacters[k]) {
                badCharacter = true;
                break;
            }
        }
        if (!badCharacter) {
            m_buffer[j++] = each;
        }
    }

    m_buffer[j] = _T('\0');
    m_buffer.resize(j + 1);
}

void StringStorage::remove(size_t startIndex, size_t count)
{
    bool isFaild = startIndex + count > getLength();
    _ASSERT(!isFaild); //is true go on
    if (isFaild) { //release version
        throw Exception(_T("An incorrect StringStorage::remove() usage"));
    }
    BufferType newBuffer = m_buffer;
    size_t copyCount = getSize() - (startIndex + count) * sizeof(TCHAR);
    memcpy(&newBuffer[startIndex], &newBuffer[startIndex + count], copyCount);
    setString(&newBuffer.front());

}

void StringStorage::truncate(size_t count)
{
    count = min(getLength(), count);
    remove(getLength() - count, count);
}

void StringStorage::toLowerCase()
{
    size_t length = getLength();
    for (size_t i = 0; i < length; i++) {
        if (_istalpha(m_buffer[i])) {
            m_buffer[i] = _totlower(m_buffer[i]);
        }
    }
}

void StringStorage::toUpperCase()
{
    _tcsupr_s(&m_buffer.front(), getLength() + 1);
}

void StringStorage::format(const TCHAR* format, ...)
{
    va_list vl;
    va_start(vl, format);

    int count = _vsctprintf(format, vl);
    va_end(vl);

    resizeBuffer(count + 1);
    va_start(vl, format);
    _vstprintf_s(&m_buffer.front(), count + 1, format, vl);
    va_end(vl);
}

void StringStorage::operator=(const StringStorage& other)
{
    setString(other.getString());
}

bool StringStorage::operator==(const StringStorage& str) const
{
    return isEqualTo(&str);
}

bool StringStorage::operator<(const StringStorage& str) const
{
    return _tcscmp(getString(), str.getString()) < 0;
}

void StringStorage::operator+=(const TCHAR* str)
{
    appendString(str);
}

void StringStorage::replaceChar(TCHAR oldChar, TCHAR newChar)
{
    size_t length = getLength();
    for (size_t i = 0; i < length; i++) {
        if (m_buffer[i] == oldChar) {
            m_buffer[i] = newChar;
        }
    }
}

void StringStorage::resizeBuffer(size_t newSize)
{
    m_buffer.resize(newSize);
#ifdef _DEBUG
    m_readableString = &m_buffer.front();
#endif
}
