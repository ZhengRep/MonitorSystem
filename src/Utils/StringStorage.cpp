#include "StringStorage.h"
#include "CommonHeader.h"
#include "Exception.h"
#include<stdlib.h>
#include<crtdbg.h>
#include<algorithm>

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
    return getLength == 0;
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

void StringStorage::getSubString(StringStorage* substr, size_t startIndex, size_t endIndex) const
{
    endIndex++;
    startIndex = std::min(startIndex, getLength());
    endIndex = std::min(endIndex, getLength());
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
    return _tcscmp(getString(), other) == 0;
}

bool StringStorage::isEqualTo(const StringStorage* other) const
{
    return isEqualTo(other->getString());
}

bool StringStorage::split(const TCHAR* delimiters, StringStorage* stringArray, size_t* arrayLength) const
{
    //special case for empty string
    if (this->getLength() == 0) {
        *arrayLength = 0;
        return true;
    }
    StringStorage copy(this->getString());
    size_t chunksCount = 0;
    size_t index = 0;
    do {
        StringStorage chunk(_T(""));
        index = copy.findOneOf(delimiters);
        
        //case when no delimiter found
        if ((index == (size_t)-1) && (chunksCount == 0)) {
            if (arrayLength != 0) {
                if (stringArray != 0) {
                    if (*arrayLength < 1) {
                        return false;
                    }
                }
                *arrayLength = 1;
            }
            else {
                return false;
            }

            if (stringArray != 0) {
                stringArray->setString(getString());
            }
            return true;
        }

        if (index == -1) {
            chunk = copy;
        }
        else {
            copy.getSubString(&chunk, 0, index - 1);
        }

        copy.getSubString(&copy, index + 1, copy.getLength() - 1);
        if ((stringArray != NULL) && (chunksCount >= *arrayLength)) {
            return false;
        }
        else if (stringArray != NULL) {
            stringArray[chunksCount] = chunk;
        }

        chunksCount++;
    } while (index != -1);

    *arrayLength = chunksCount;
    return true;
}

TCHAR* StringStorage::find(const TCHAR* subStr)
{
    return nullptr;
}

size_t StringStorage::findChar(const TCHAR c)
{
    return size_t();
}

size_t StringStorage::findOneOf(const TCHAR* string)
{
    return size_t();
}

size_t StringStorage::findLast(const TCHAR c)
{
    return size_t();
}

void StringStorage::removeChars(const TCHAR badCharacters[], size_t count)
{
}

void StringStorage::remove(size_t startIndex, size_t count)
{
}

void StringStorage::truncate(size_t count)
{
}

void StringStorage::toLowerCase()
{
}

void StringStorage::toUpperCase()
{
}

void StringStorage::format(const TCHAR* format, ...)
{
}

void StringStorage::operator=(const StringStorage& other)
{
}

bool StringStorage::operator==(const StringStorage& str) const
{
    return false;
}

bool StringStorage::operator<(const StringStorage& str) const
{
    return false;
}

void StringStorage::operator+=(const TCHAR* str)
{
}

void StringStorage::replaceChar(TCHAR oldChar, TCHAR newChar)
{
}

void StringStorage::resizeBuffer(size_t newSize)
{
    m_buffer.resize(newSize);
#ifdef _DEBUG
    m_readableString = &m_buffer.front();
#endif
}
