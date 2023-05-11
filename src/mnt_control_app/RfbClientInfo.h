#pragma once
#include "Utils/CommonHeader.h"
#include <list>

class RfbClientInfo
{
public:
  RfbClientInfo(UINT32 id, const TCHAR* peerAddr);
  virtual ~RfbClientInfo();

public:
  UINT32 m_id;
  StringStorage m_peerAddr;
};

typedef std::list<RfbClientInfo> RfbClientInfoList;

