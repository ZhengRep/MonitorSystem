#pragma once
#include "Utils/CommonHeader.h"
#include "NamedPipe.h"

class PipeClient
{
public:
  static NamedPipe* connect(const TCHAR* name, unsigned int maxPortionSize) throw(Exception);

private:
  PipeClient(){};

  unsigned int m_maxPortionSize;
};

