#pragma once
class RfbClientManagerEventListener
{
public:
  virtual ~RfbClientManagerEventListener(){};
  virtual void afterFirstClientConnect() = 0;
  virtual void afterLastClientDisconnect() = 0;
};

