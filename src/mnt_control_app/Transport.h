#pragma once
#include "io_lib/Channel.h"
#include "Utils/Exception.h"

class Transport
{
public:
  virtual ~Transport(){};
  virtual Channel* getIOStream() = 0;
  virtual Transport* accept() = 0;
  virtual void close() throw(Exception) = 0;
};

