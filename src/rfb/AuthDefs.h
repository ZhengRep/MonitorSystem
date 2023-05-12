#pragma once
#include "utils/inttypes.h"

class SecurityDefs
{
public:
  static const UINT32 INVALID = 0;
  static const UINT32 NONE = 1;
  static const UINT32 MNT = 2;
  static const UINT32 TIGHT = 16;
  static UINT32 convertFromAuthType(UINT32 authType);
};

class AuthDefs
{
public:
  static const UINT32 NONE = 1;
  static const UINT32 MNT = 2;
  static const UINT32 EXTERNAL = 130;

  static const char* const SIG_NONE;
  static const char* const SIG_MNT;
  static const char* const SIG_EXTERNAL;

  static UINT32 convertFromSecurityType(UINT32 securityType);
};

