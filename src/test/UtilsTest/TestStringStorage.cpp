#include "../../Utils/StringStorage.h"

int main()
{
	
	StringStorage str(_T("Hello World. I am Zheng!"));
	StringStorage words[10];
	size_t arrayLength = 10;
	str.split(_T(" "), words, &arrayLength);

	return 0;
}