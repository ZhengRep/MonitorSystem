#pragma once
#include "RegistryKey.h"

class Registry
{
public:
  const static int STRING_TYPE = REG_SZ;
  const static int EXPAND_STRING_TYPE = REG_EXPAND_SZ;
  const static int BINARY_TYPE = REG_BINARY;
  const static int DWORD_TYPE = REG_DWORD;
  const static int QWORD_TYPE = REG_QWORD;

public:
  static HKEY getCurrentUserKey();
  static HKEY getCurrentLocalMachineKey();

private:
  Registry(){};
  ~Registry(){};
};

