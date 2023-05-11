#pragma once
#include "io_lib/Channel.h"

class ConnectionListener
{
public:
  virtual void onNewConnection(Channel* channel) = 0;
};

