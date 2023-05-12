#pragma once
#include "security/SecurityDescriptor.h"
#include "security/SecurityIdentifier.h"
#include <vector>

class SecurityAttributes
{
public:
  SecurityAttributes();
  virtual ~SecurityAttributes();

  void setDefaultAttributes();
  void shareToAllUsers();
  SECURITY_ATTRIBUTES* getSecurityAttributes();
  void setInheritable();

private:
  SECURITY_ATTRIBUTES m_securityAttributes;
  bool m_isDefaultAttributes;
  SecurityDescriptor m_sd;
};

