#pragma once
#include "io_lib/Channel.h"
#include "TimerListener.h"

class SecurityPipeServer : private TimerListener
{
public:
  SecurityPipeServer(Channel* tempPublChan, unsigned int bufferSize);
  virtual ~SecurityPipeServer();
  Channel* getChannel();

private:
  virtual void onTimeTimer();

  void generateSecConnection(Channel* tempPublChan);
  void makeSure();

  Channel* m_secChannel;
  unsigned int m_bufferSize;
};

