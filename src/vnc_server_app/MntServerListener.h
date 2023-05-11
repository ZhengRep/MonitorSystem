#pragma once
class MntServerListener
{
public:
  virtual ~MntServerListener(){};
  virtual void onMntServerShutdown() = 0;
};

