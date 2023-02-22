#include "Shell.h"

void Shell::runAsAdmin(const TCHAR* pathToFile, const TCHAR* parameters) throw(SystemException)
{
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(sei));

	sei.cbSize = sizeof(SHELLEXECUTEINFOW);
	sei.hwnd = 0;
	sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = _T("runas");
	sei.lpFile = pathToFile;
	sei.lpParameters = parameters;
	sei.nShow = SW_SHOWNORMAL;

	if (ShellExecuteEx(&sei) == FALSE) {
		throw SystemException();
	}

	WaitForSingleObject(sei.hProcess, INFINITE);
	CloseHandle(sei.hProcess);
}

void Shell::open(const TCHAR* file, const TCHAR* parameters, const TCHAR* workDirectory) throw(SystemException)
{
	int ret = (int)ShellExecute(0, _T("open"), file, parameters, workDirectory, SW_SHOW);
	if (ret <= 32) {
		throw SystemException(ret);
	}
}

Shell::Shell()
{
}
