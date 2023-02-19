#include "GetCPUTime.h"
#include<time.h>
#include<Windows.h>

double getCPUTime()
{
	FILETIME createTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;
	if (GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime) != -1) {
		ULARGE_INTEGER li = { {userTime.dwLowDateTime, userTime.dwHighDateTime } };
		return li.QuadPart / 10000000.;
	}

	return -1;        /* Failed. */
}

double getKernelTime()
{
#if defined(_WIN32)
	/* Windows -------------------------------------------------- */
	FILETIME createTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;
	if (GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime) != -1) {
		ULARGE_INTEGER li = { { kernelTime.dwLowDateTime, kernelTime.dwHighDateTime } };
		return li.QuadPart / 10000000.;
	}
#endif
	return 0.0;
}
