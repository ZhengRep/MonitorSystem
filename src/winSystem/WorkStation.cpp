#include "WorkStation.h"

void Workstation::lock()
{
  if (LockWorkStation() == FALSE) {
    throw SystemException();
  }
}

void Workstation::logOff()
{
  if (ExitWindowsEx(EWX_LOGOFF, SHTDN_REASON_MAJOR_APPLICATION) == FALSE) {
    throw SystemException();
  }
}
