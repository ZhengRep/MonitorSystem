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
}

void AnsiStringStorage::format(const char* format, ...)
{
}

void AnsiStringStorage::appendString(const char* string)
{
}

bool AnsiStringStorage::checkAnsiConversion(StringStorage& string)
{
	return false;
}
