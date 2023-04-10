#pragma once
#include "RfbDispatcher.h"
#include "CapContainer.h"

class RfbCodeRegistrator
{
public:
  RfbCodeRegistrator(RfbDispatcher* dispatcher, CapContainer* srvToClCaps, CapContainer* clToSrvCaps, CapContainer* encCaps);
  virtual ~RfbCodeRegistrator();

  void regCode(UINT32 code, RfbDispatcherListener* listener);
  void addSrvToClCap(UINT32 code, const char* vendorSignature, const char* nameSignature);
  void addClToSrvCap(UINT32 code, const char* vendorSignature, const char* nameSignature);
  void addEncCap(UINT32 code, const char* vendorSignature, const char* nameSignature);

private:
  RfbDispatcher* m_dispatcher;
  CapContainer* m_srvToClCaps;
  CapContainer* m_clToSrvCaps;
  CapContainer* m_encCaps;
};

