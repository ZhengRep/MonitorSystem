#include "RfbClientInfo.h"

RfbClientInfo::RfbClientInfo(UINT32 id, const TCHAR* peerAddr)
  :m_id(id), m_peerAddr(peerAddr)
{
}

RfbClientInfo::~RfbClientInfo()
{
}
