#pragma once
#include "io_lib/Channel.h"

class SecurityPipeClient
{
public:
  SecurityPipeClient(Channel* tempPublChan, unsigned int maxPortionSize);
  virtual ~SecurityPipeClient();
  Channel* getChannel();

private:
  Channel* m_secChannel;
};

