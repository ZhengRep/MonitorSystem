#pragma once
#include "SystemException.h"

class Workstation
{
public:
 
  static void lock() throw(SystemException);
  static void logOff() throw(SystemException);
};

