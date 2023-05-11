#pragma once

class ClientTerminationListener
{
public:
  virtual ~ClientTerminationListener() {}

  virtual void onClientTerminate() = 0;
};