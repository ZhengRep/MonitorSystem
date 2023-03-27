#pragma once
#include "region/Rect.h"

class UpdateRequestListener
{
public:
  virtual ~UpdateRequestListener(){}
  virtual void onUpdateRequest(const Rect* rectRequested, bool incremental) = 0;
};

