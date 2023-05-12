#pragma once
#include "utils/CommonHeader.h"

class LogInitListener
{
public:
  virtual void onLogInit(const TCHAR* logDir, const TCHAR* fileName, unsigned char logLevel) = 0;
  virtual void onChangeLogProps(const TCHAR* newLogDir, unsigned char newLevel) = 0;
};

