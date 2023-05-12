#pragma once
#include "utils/CommonHeader.h"
#include "utils/Exception.h"

class WindowsSocket
{
public:
  static void startup(BYTE loVer, BYTE hiVer) throw(Exception);
  static void cleanup() throw(Exception);

protected:
  static bool m_isStarted;
};

