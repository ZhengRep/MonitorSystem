#pragma once
#include "ControlGate.h"
#include "ControlAuthException.h"
#include "server_config_lib/ServerConfig.h"

class ControlAuth
{
public:

  ControlAuth(ControlGate* gate, const TCHAR* password)
    throw(IOException, ControlAuthException, Exception);

  virtual ~ControlAuth();

protected:
  void authRfb() throw(IOException, ControlAuthException);

protected:
  ControlGate* m_gate;

  UINT8 m_password[ServerConfig::MNT_PASSWORD_SIZE];
};

