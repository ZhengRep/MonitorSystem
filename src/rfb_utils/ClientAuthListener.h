#pragma once
#include "desktop/Desktop.h"
#include "RfbClient.h"
#include "AuthException.h"

class ClientAuthListener
{
public:
  virtual ~ClientAuthListener() {}

  virtual Desktop* onClientAuth(RfbClient* client) = 0;
  virtual bool onCheckForBan(RfbClient* client) = 0;
  virtual void onAuthFailed(RfbClient* client) = 0;
  virtual void onCheckAccessControl(RfbClient* client) throw(AuthException) = 0;
};