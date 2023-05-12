#include "SecurityIdentifier.h"
#include <Sddl.h>

SecurityIdentifier::SecurityIdentifier(SID* sid) throw(SystemException)
{
  m_sid = (SID*)LocalAlloc(LPTR, SECURITY_MAX_SID_SIZE);
  memset(m_sid, 0, SECURITY_MAX_SID_SIZE);

  if (!CopySid(SECURITY_MAX_SID_SIZE, m_sid, sid)) {
    throw SystemException();
  }
}

SecurityIdentifier::SecurityIdentifier(const TCHAR* sidString)
  : m_sid(0)
{
  getSidByString(sidString, (PSID*)&m_sid);
}

SecurityIdentifier::~SecurityIdentifier()
{
  LocalFree(m_sid);
}

bool SecurityIdentifier::isValid()
{
  return IsValidSid(m_sid) == TRUE;
}

void SecurityIdentifier::toString(StringStorage* sidString) throw(SystemException)
{
  TCHAR* localAllocatedSidString;

  if (ConvertSidToStringSid(m_sid, &localAllocatedSidString) == FALSE) {
    throw SystemException();
  }

  sidString->setString(localAllocatedSidString);

  LocalFree(localAllocatedSidString);
}

SecurityIdentifier* SecurityIdentifier::getProcessOwner(HANDLE processHandle) throw(SystemException)
{
  HANDLE procToken;

  if (OpenProcessToken(processHandle, TOKEN_QUERY, &procToken)) {
    try {
      char buffer[1024];
      DWORD retLen = 0;
      if (!GetTokenInformation(procToken, TokenUser, &buffer, sizeof(buffer), &retLen)) {
        throw SystemException();
      }
      CloseHandle(procToken);
      return new SecurityIdentifier((SID*)((TOKEN_USER*)buffer)->User.Sid);
    } catch (...) {
      CloseHandle(procToken);
      throw;
    }
    CloseHandle(procToken);
  } else {
    throw SystemException();
  }
}

SecurityIdentifier* SecurityIdentifier::createSidFromString(const TCHAR* sidString) throw(SystemException)
{
  return new SecurityIdentifier(sidString);
}

SID* SecurityIdentifier::getSid() const
{
  return m_sid;
}

void SecurityIdentifier::getSidByString(const TCHAR* sidString, PSID* sid)
{
  if (ConvertStringSidToSid(sidString, sid) == FALSE) {
    throw SystemException();
  }

  _ASSERT(IsValidSid(*sid));
}
