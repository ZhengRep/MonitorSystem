#include "Utils/UnicodeStringStorage.h"
#include<wchar.h>
#include<iostream>

int main()
{
	StringStorage stringStorage(_T("Zheng"));
	size_t length = stringStorage.getLength(); //length is used to characters
	size_t size = stringStorage.getSize(); //size is used to bytes
	/*UnicodeStringStorage unicodeStringStorage(L"Hello, these are unicode");
	unicodeStringStorage.toStringStorage(&stringStorage);*/

	return 0;
}