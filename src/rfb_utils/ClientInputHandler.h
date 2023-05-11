#pragma once
#include "RfbDispatcherListener.h"
#include "RfbCodeRegistrator.h"
#include "ClientInputEventListener.h"


class ClientInputHandler: public RfbDispatcherListener
{
public:
  ClientInputHandler(RfbCodeRegistrator* codeRegtor, ClientInputEventListener* extEventListener, bool viewOnly);
  virtual ~ClientInputHandler();

  void setViewOnlyFlag(bool value) { m_viewOnly = value; }
protected:
  // Listen function
  virtual void onRequest(UINT32 reqCode, RfbInputGate* input);

  ClientInputEventListener* m_extEventListener;
  bool m_viewOnly;
};

