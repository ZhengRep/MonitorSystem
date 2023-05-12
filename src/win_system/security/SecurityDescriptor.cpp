#include "SecurityDescriptor.h"
#include <AclAPI.h>

SecurityDescriptor::SecurityDescriptor()
{
  InitializeSecurityDescriptor(&m_sd, SECURITY_DESCRIPTOR_REVISION);
}

SecurityDescriptor::~SecurityDescriptor()
{
}

void SecurityDescriptor::setRulesAsDacl(size_t count, EXPLICIT_ACCESS* rules) throw(SystemException)
{
  PACL acl = 0;

  ULONG constrCount = (ULONG)count;
  _ASSERT(constrCount == count);
  DWORD ret = SetEntriesInAcl(constrCount, rules, NULL, &acl);

  if (ret != ERROR_SUCCESS) {
    throw SystemException(ret);
  }

  setUserDacl(acl);
}

void SecurityDescriptor::setUserDacl(ACL* acl) throw(SystemException)
{
  if (SetSecurityDescriptorDacl(&m_sd, TRUE, acl, FALSE) == FALSE) {
    throw SystemException();
  }
}

void SecurityDescriptor::clearOwner()
{
  SetSecurityDescriptorOwner(&m_sd, 0, TRUE);
}

bool SecurityDescriptor::isValid()
{
  return IsValidSecurityDescriptor(&m_sd) == TRUE;
}

SECURITY_DESCRIPTOR* SecurityDescriptor::getSD()
{
  return &m_sd;
}
