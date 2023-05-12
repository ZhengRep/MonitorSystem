#pragma once
#include "utils/CommonHeader.h"

class ConnectStringParser
{
public:
  ConnectStringParser();
  virtual ~ConnectStringParser();

  static bool parse(const TCHAR* connectString, StringStorage* host = 0,
    unsigned short* port = 0);
};

