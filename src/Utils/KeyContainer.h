#pragma once
#include "CommonHeader.h"

class KeyContainer
{
public:
  KeyContainer();

  StringStorage key;
  bool isArgument;
  StringStorage argument;
};

