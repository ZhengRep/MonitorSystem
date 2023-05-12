#pragma once
#include "utils/winhdr.h"
#include "win_system/SystemException.h"
#include <AccCtrl.h>

//Contains the security information associated with an object
class SecurityDescriptor
{
public:
  /**
   * Creates new security descriptor.
   * @remark created security descriptor have no system access control list (SACL),
   * no discretionary access control list (DACL), no owner, no primary group,
   * and all control flags set to FALSE (NULL). Thus, except for its revision level, it is empty
   */
  SecurityDescriptor();
  virtual ~SecurityDescriptor();

  void setRulesAsDacl(size_t count, EXPLICIT_ACCESS* rules) throw(SystemException);
  void setUserDacl(ACL* acl) throw(SystemException);
  void clearOwner();
  bool isValid();
  SECURITY_DESCRIPTOR* getSD();

private:
  SECURITY_DESCRIPTOR m_sd;
};

