#pragma once

class AnEventListener {
public:
  AnEventListener(){}
  virtual ~AnEventListener(){}
  virtual void onAnObjectEvent() = 0;
};