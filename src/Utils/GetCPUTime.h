#pragma once

double getCPUTime();

double getKernelTime();

//return current processor tick number
inline unsigned long long rdtsc() {
	unsigned int lo, hi;
#ifdef __GNUC__
	asm volatile("rdtsc\n":"-a", "=d"(hi));
#elif _MSC_VER
#ifdef _M_IX86
	_asm {
		rdstc
		mov DWORD PTR [lo], eax
		mov DWORD PTR [hi], edx
	}
#else
	return 0;
#endif
#else
#error "Unsupported compiler"
#endif
	return ((unsigned long long)hi << 32) | lo;
}

class GetCPUTime
{
};

