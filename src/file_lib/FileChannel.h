#pragma once
#include "io_lib/Channel.h"

class FileChannel: public Channel
{
public :
  virtual void seek(INT64 n) = 0;
};

