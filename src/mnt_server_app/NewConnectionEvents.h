#pragma once
#include "utils/CommonHeader.h"

class NewConnectionEvents
{
public:
  virtual void onSuccAuth(const StringStorage* ip) = 0;
  virtual void onAuthFailed(const StringStorage* ip) = 0;
  virtual void onDisconnect(const StringStorage* message) = 0;
};

