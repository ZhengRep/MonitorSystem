#pragma once
#include "network/RfbInputGate.h"

class RfbDispatcherListener
{
public:
  virtual ~RfbDispatcherListener() {};
  virtual void onRequest(UINT32 reqCode, RfbInputGate* input) = 0;
};

