#pragma once
#include "io_lib/Channel.h"

class ReconnectionListener
{
public:
  ReconnectionListener() {}
  virtual ~ReconnectionListener() {}

  virtual void onReconnect(Channel* newChannelTo, Channel* newChannelFrom) = 0;
};

