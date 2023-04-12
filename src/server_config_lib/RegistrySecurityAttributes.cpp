#include "RegistrySecurityAttributes.h"

RegistrySecurityAttributes::RegistrySecurityAttributes() throw(SystemException)
{
  EXPLICIT_ACCESS serviceRules[2];
  EXPLICIT_ACCESS applicationRules[2];
  ZeroMemory(&serviceRules, sizeof(serviceRules));
  ZeroMemory(&applicationRules, sizeof(applicationRules));
  fillExplicitAccesses(serviceRules, applicationRules);

  m_serviceSD.setRulesAsDacl(2, serviceRules);
  m_applicationSD.setRulesAsDacl(2, applicationRules);

  m_serviceSA.nLength = sizeof(SECURITY_ATTRIBUTES);
  m_serviceSA.lpSecurityDescriptor = m_serviceSD.getSD();
  m_serviceSA.bInheritHandle = FALSE;

  m_applicationSA.nLength = sizeof(SECURITY_ATTRIBUTES);
  m_applicationSA.lpSecurityDescriptor = m_applicationSD.getSD();
  m_applicationSA.bInheritHandle = FALSE;
}

RegistrySecurityAttributes::~RegistrySecurityAttributes()
{
  delete m_admins;
  delete m_serviceOwner;
  delete m_applicationOwner;
}

void RegistrySecurityAttributes::fillExplicitAccess(EXPLICIT_ACCESS* rules,
  const SecurityIdentifier* secIdent)
{
  rules->grfAccessPermissions = KEY_ALL_ACCESS;
  rules->grfAccessMode = SET_ACCESS;
  rules->grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
  rules->Trustee.TrusteeForm = TRUSTEE_IS_SID;
  rules->Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
  rules->Trustee.ptstrName = (LPTSTR)secIdent->getSid();
}

void RegistrySecurityAttributes::fillExplicitAccesses(EXPLICIT_ACCESS serviceRules[2],
  EXPLICIT_ACCESS applicationRules[2])
{
  m_admins = SecurityIdentifier::createSidFromString(_T("S-1-5-32-544"));
  m_serviceOwner = SecurityIdentifier::createSidFromString(_T("S-1-5-18"));
  m_applicationOwner = SecurityIdentifier::getProcessOwner(GetCurrentProcess());

  _ASSERT(m_admins->isValid());
  _ASSERT(m_serviceOwner->isValid());
  _ASSERT(m_applicationOwner->isValid());

  // All access for service.
  fillExplicitAccess(&serviceRules[0], m_admins);
  fillExplicitAccess(&serviceRules[1], m_serviceOwner);
  // All access for application.
  fillExplicitAccess(&applicationRules[0], m_admins);
  fillExplicitAccess(&applicationRules[1], m_applicationOwner);
}

SECURITY_ATTRIBUTES* RegistrySecurityAttributes::getServiceSA()
{
  return &m_serviceSA;
}

SECURITY_ATTRIBUTES* RegistrySecurityAttributes::getApplicationSA()
{
  return &m_applicationSA;
}

