#pragma once
#include "BlockingGate.h"

class ClientListener
{
public:
  ClientListener(){};
  virtual ~ClientListener(){};

  virtual void onRequest(UINT8 reqCode, BlockingGate* backGate) = 0;
};

