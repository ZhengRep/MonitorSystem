#pragma once
#include "StringStorage.h"

class UnicodeStringStorage
{
public:
	UnicodeStringStorage();
	UnicodeStringStorage(const WCHAR* string);
	UnicodeStringStorage(const StringStorage* string);
	UnicodeStringStorage(const UnicodeStringStorage& stringBuffer);
	~UnicodeStringStorage();

	void fromStringStorage(const StringStorage* src);
	void toStringStorage(StringStorage* dst);

	// Fills internal buffer by the string.
	virtual void setString(const WCHAR* string);
	const WCHAR* getString() const;

	//Returns length of string(in characters, not including terminating null character)
	size_t getLength() const;
	//Returns size of string in bytes, including terminating null character
	size_t getSize() const;
	bool isEmpty() const;

private:
	typedef std::vector<WCHAR> BufferType;
	BufferType m_buffer;
};

