#pragma once
#include "rfb/CursorShape.h"
#include "rfb/FrameBuffer.h"
#include "UpdateContainer.h"

class UpdateSendingListener
{
public:
  virtual ~UpdateSendingListener() {}
  virtual void onSendUpdate(const UpdateContainer* updateContainer, const CursorShape* cursorShape) = 0;
  virtual bool isReadyToSend() = 0;

};

