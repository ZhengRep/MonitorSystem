#include<iostream>

int main()
{
	union {
		char test;
		int i;
	}testBigEndian;
	testBigEndian.i = 1;
	return 0;
}