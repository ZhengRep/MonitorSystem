#pragma once
#include "win_system/security/SecurityIdentifier.h"
#include "win_system/security/SecurityDescriptor.h"

#include <AccCtrl.h>
#include <AclAPI.h>

//Contains security attributes (security descriptor) for protecting
//vnc server and service configuration
class RegistrySecurityAttributes
{
public:
  RegistrySecurityAttributes() throw(SystemException);
  virtual ~RegistrySecurityAttributes();

  SECURITY_ATTRIBUTES* getServiceSA();
  SECURITY_ATTRIBUTES* getApplicationSA();

private:
  void fillExplicitAccess(EXPLICIT_ACCESS* rules, const SecurityIdentifier* secIdent);
  void fillExplicitAccesses(EXPLICIT_ACCESS serviceRules[2], EXPLICIT_ACCESS applicationRules[2]);

  SecurityIdentifier* m_admins;
  SecurityIdentifier* m_serviceOwner;
  SecurityIdentifier* m_applicationOwner;

  SecurityDescriptor m_serviceSD;
  SecurityDescriptor m_applicationSD;
  SECURITY_ATTRIBUTES m_serviceSA;
  SECURITY_ATTRIBUTES m_applicationSA;
};

