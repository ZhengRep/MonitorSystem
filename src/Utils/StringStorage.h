#pragma once

#include "winhdr.h"
#include<vector>

class StringStorage
{
public:
	StringStorage();
	StringStorage(const TCHAR* string);
	StringStorage(const StringStorage& stringBuffer);
	virtual ~StringStorage();
	virtual void setString(const TCHAR* string);
	const TCHAR* getString() const;
	size_t getLength() const;
	size_t getSize() const; //include terminateing null character
	bool isEmpty() const;
	bool endsWith(TCHAR postfix) const;
	bool beginsWith(TCHAR prefix) const;
	void getSubString(StringStorage* substr, size_t startIndex, size_t endIndex) const;
	void appendChar(TCHAR c);
	void appendString(const TCHAR* string);
	void quoteSelf();
	bool isEqualTo(const TCHAR* other) const;
	bool isEqualTo(const StringStorage* other) const;
	bool split(const TCHAR* delimiters, StringStorage* stringArray, size_t* arrayLength) const;
	TCHAR* find(const TCHAR* subStr);
	size_t findChar(const TCHAR c);
	size_t findOneOf(const TCHAR* string);
	size_t findLast(const TCHAR c);
	void removeChars(const TCHAR badCharacters[], size_t count);
	void remove(size_t startIndex, size_t count);
	void truncate(size_t count);
	void toLowerCase();
	void toUpperCase();
	void format(const TCHAR* format, ...);
	void operator = (const StringStorage& other);
	bool operator == (const StringStorage& str) const;
	bool operator < (const StringStorage& str) const;
	void operator += (const TCHAR* str);

	void replaceChar(TCHAR oldChar, TCHAR newChar);

private:
	typedef std::vector<TCHAR> BufferType;
	BufferType m_buffer;
	void resizeBuffer(size_t newSize);

#ifdef _DEBUG
	TCHAR* m_readableString;
#endif
};


