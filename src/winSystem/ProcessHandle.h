#pragma once

#include "Utils/CommonHeader.h"

class ProcessHandle
{
public:
	ProcessHandle();
	virtual ~ProcessHandle();

	void openProcess(DWORD desiredAccess, BOOL inheritedHandle, DWORD processId);

	HANDLE getHandle() const;
	void getProcessModulePath(StringStorage* exePath);

private:
	HANDLE m_hProcess;
};

