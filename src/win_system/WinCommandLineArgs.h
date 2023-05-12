#pragma once
#include "utils/CommandLineArgs.h"

class WinCommandLineArgs : public CommandLineArgs
{
public:
  WinCommandLineArgs(const TCHAR* cmdLineInWinFormat);
  virtual ~WinCommandLineArgs();
};
