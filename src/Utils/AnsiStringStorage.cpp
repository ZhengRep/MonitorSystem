#include "AnsiStringStorage.h"
#include "CommonHeader.h"
#include "Exception.h"
#include<crtdbg.h>

AnsiStringStorage::AnsiStringStorage()
{
	setString("");
}

AnsiStringStorage::AnsiStringStorage(const char* string)
{
	setString(string);
}

AnsiStringStorage::AnsiStringStorage(const StringStorage* string)
{
	fromStringStorage(string);
}

AnsiStringStorage::AnsiStringStorage(const AnsiStringStorage& stringBuffer)
{
	*this = stringBuffer;
}

AnsiStringStorage::~AnsiStringStorage()
{
}

void AnsiStringStorage::setString(const char* string)
{
	if (!string) {
		string = "";
	}
	size_t length = strlen(string);
	m_buffer.resize(length + 1);
	memcpy(&m_buffer.front(), string, getSize());
}

const char* AnsiStringStorage::getString() const
{
	return &m_buffer.front();
}

size_t AnsiStringStorage::getLength() const
{
	return m_buffer.size() - 1;
}

size_t AnsiStringStorage::getSize() const
{
	return m_buffer.size();
}

bool AnsiStringStorage::isEmpty() const
{
	return m_buffer.size() == 0;
}

void AnsiStringStorage::fromStringStorage(const StringStorage* src)
{
#ifndef	_UNICODE
	setString(src->getString());
#else
	int symbolCount = WideCharToMultiByte(CP_ACP, 0, src->getString(),
		-1, NULL, 0, NULL, NULL);
	
	m_buffer.resize(symbolCount);

	WideCharToMultiByte(CP_ACP, 0, src->getString(), -1, &m_buffer.front(),
		symbolCount, NULL, NULL);

#endif
}

void AnsiStringStorage::toStringStorage(StringStorage* dst) const
{
#ifndef _UNICODE
	dst->setString(getString());
#else
	int symbolCount = getSize();
	std::vector<WCHAR> unicodeBuffer(symbolCount);
	int result = MultiByteToWideChar(CP_ACP, 0, &m_buffer.front(), symbolCount,
		&unicodeBuffer.front(), symbolCount);

	if (result == 0) {
		throw Exception(_T("Cannot convert from Ansi to StringStorage"));
	}
	dst->setString(&unicodeBuffer.front());
#endif
}

void AnsiStringStorage::format(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);
	int count = _vscprintf(format, vl);
	va_end(vl);

	m_buffer.resize(count + 1);

	va_start(vl, format);
	vsprintf_s(&m_buffer.front(), count + 1, format, vl);
	va_end(vl);
}

void AnsiStringStorage::appendString(const char* string)
{
	if (string == NULL) {
		return;
	}

	AnsiStringStorage src(string);
	BufferType::iterator to = m_buffer.begin() + getLength(); //this uses length to pointer null character
	BufferType::iterator fromFirst = src.m_buffer.begin();
	BufferType::iterator fromLast = src.m_buffer.begin() + src.getLength();
	
	m_buffer.insert(to, fromFirst, fromLast);
}

bool AnsiStringStorage::checkAnsiConversion(StringStorage& string)
{
	AnsiStringStorage ansi(&string);
	StringStorage check;
	ansi.toStringStorage(&check);
	return string.isEqualTo(&check);
}
