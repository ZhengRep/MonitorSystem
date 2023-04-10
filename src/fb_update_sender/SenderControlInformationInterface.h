#pragma once
#include "region/Rect.h"
#include "region/Region.h"
//Provide necessary information to send a information

class SenderControlInformationInterface {
public:
  virtual void onGetViewPort(Rect* viewRect, bool* shareApp, Region* shareAppRegion) = 0;
};