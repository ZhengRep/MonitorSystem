#include "Registry.h"

HKEY Registry::getCurrentUserKey()
{
  return HKEY_CURRENT_USER;
}

HKEY Registry::getCurrentLocalMachineKey()
{
  return HKEY_LOCAL_MACHINE;
}
