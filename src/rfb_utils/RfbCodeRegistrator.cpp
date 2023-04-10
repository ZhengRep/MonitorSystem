#include "RfbCodeRegistrator.h"

RfbCodeRegistrator::RfbCodeRegistrator(RfbDispatcher* dispatcher, CapContainer* srvToClCaps, CapContainer* clToSrvCaps, CapContainer* encCaps)
  : m_dispatcher(dispatcher),
    m_srvToClCaps(srvToClCaps),
    m_clToSrvCaps(clToSrvCaps),
    m_encCaps(encCaps)
{
}

RfbCodeRegistrator::~RfbCodeRegistrator()
{
}

void RfbCodeRegistrator::regCode(UINT32 code, RfbDispatcherListener* listener)
{
  m_dispatcher->registerNewHandle(code, listener);
}

void RfbCodeRegistrator::addSrvToClCap(UINT32 code, const char* vendorSignature, const char* nameSignature)
{
  m_srvToClCaps->addCap(code, vendorSignature, nameSignature);
}

void RfbCodeRegistrator::addClToSrvCap(UINT32 code, const char* vendorSignature, const char* nameSignature)
{
  m_clToSrvCaps->addCap(code, vendorSignature, nameSignature);
}

void RfbCodeRegistrator::addEncCap(UINT32 code, const char* vendorSignature, const char* nameSignature)
{
  m_encCaps->addCap(code, vendorSignature, nameSignature);
}
