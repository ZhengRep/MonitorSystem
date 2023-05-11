#pragma once
#include "LogConn.h"

class LogConnAuthListener
{
public:
  virtual FileAccountHandle onLogConnAuth(LogConn* logConn, bool success, 
    const TCHAR* fileName) = 0;
  virtual void onDisconnect(LogConn* logConn) = 0;
};

