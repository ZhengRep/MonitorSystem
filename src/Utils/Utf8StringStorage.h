#pragma once
#include "StringStorage.h"

//only constains utf8 symbols "as is". do not add termination symbol(s) and do not remove
//when it already presented.
class Utf8StringStorage
{
public:
	Utf8StringStorage();
	Utf8StringStorage(const std::vector<char>* utf8Buffer);
	Utf8StringStorage(const StringStorage* string);
	Utf8StringStorage(const Utf8StringStorage& string);
	~Utf8StringStorage();

	// Sets internal buffer by an external utf8 string.
	void setString(const std::vector<char>* utf8Buffer);
	const char* getString() const;
	size_t getSize() const;
	size_t getLength() const;
	void fromStringStorage(const StringStorage* src);
	void toStringStorage(StringStorage* dst);

private:
	std::vector<char> m_buffer;
};

