#include "Impersonator.h"
#include "WTS.h"
#include <crtdbg.h>

Impersonator::Impersonator(LogWriter* log)
	:m_token(INVALID_HANDLE_VALUE),
	m_dupToken(INVALID_HANDLE_VALUE),
	m_log(log)
{
}

void Impersonator::impersonateAsLoggedUser() throw(SystemException)
{
	WTS::queryConsoleUserToken(&m_token, m_log);
	if (!DuplicateToken(m_token, SecurityImpersonation, &m_dupToken)) {
		throw SystemException(_T("Could not DuplicateToken"));
		if (!ImpersonateLoggedOnUser(m_dupToken)) {
			throw SystemException(_T("Could not ImpersonateLoggedOnUser"));
		}
	}
}

void Impersonator::revertToSelf() throw(SystemException)
{
	if (m_dupToken != INVALID_HANDLE_VALUE) {
		CloseHandle(m_dupToken);
	}

	if (m_token != INVALID_HANDLE_VALUE) {
		CloseHandle(m_token);
	}

	m_dupToken = INVALID_HANDLE_VALUE;
	m_token = INVALID_HANDLE_VALUE;

	if (!RevertToSelf()) {
		throw SystemException(_T("could not RevertToSelf"));
	}
}
