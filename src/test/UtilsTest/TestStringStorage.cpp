#include "Utils/AnsiStringStorage.h"
#include<iostream>

int main()
{
	AnsiStringStorage ansiStrFormat(_T("Hello World. I am Zheng!"));
	ansiStrFormat.appendString(_T("HOHOHO"));
	std::cout << ansiStrFormat.getString() << std::endl;
	return 0;
}