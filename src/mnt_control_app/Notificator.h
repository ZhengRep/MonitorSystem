#pragma once
#include "Utils/CommonHeader.h"

class Notificator
{
public:
  virtual ~Notificator(){};

  virtual void notifyServerSideException(const TCHAR* reason) = 0;
  virtual void notifyConnectionLost() = 0;
};

