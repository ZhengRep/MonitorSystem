#pragma once
#include "thread/LocalMutex.h"

class UpdateListener
{
public:
  UpdateListener() {};
  ~UpdateListener() {};

  virtual void onUpdate() = 0;
};

