#pragma once
#include "StringStorage.h"

class AnsiStringStorage
{
public:
	AnsiStringStorage();
	AnsiStringStorage(const char* string);
	AnsiStringStorage(const StringStorage* string);
	AnsiStringStorage(const AnsiStringStorage& stringBuffer);
	~AnsiStringStorage();

	virtual void setString(const char* string);
	const char* getString() const;
	size_t getLength() const;
	size_t getSize() const;

	// Checks if string is empty.
	// @return true if string is empty.
	bool isEmpty() const;

	void fromStringStorage(const StringStorage* src);
	void toStringStorage(StringStorage* dst) const;

	void format(const char* format, ...);

	void appendString(const char* string);

	static bool checkAnsiConversion(StringStorage& string);

private:
	typedef std::vector<char> BufferType;
	BufferType m_buffer;
};

