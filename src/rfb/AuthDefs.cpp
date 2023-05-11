#include "AuthDefs.h"

const char* const AuthDefs::SIG_NONE = "NOAUTH__";
const char* const AuthDefs::SIG_MNT = "MNTAUTH_";
const char* const AuthDefs::SIG_EXTERNAL = "XTRNAUTH";

UINT32 SecurityDefs::convertFromAuthType(UINT32 authType)
{
  switch (authType) {
  case AuthDefs::NONE:
    return NONE;
  case AuthDefs::MNT:
    return MNT;
  }

  return INVALID; // no corresponding authentication method
}

UINT32 AuthDefs::convertFromSecurityType(UINT32 securityType)
{
  switch (securityType) {
  case SecurityDefs::NONE:
    return NONE;
  case SecurityDefs::MNT:
    return MNT;
  }

  return 0; // no corresponding authentication method
}
