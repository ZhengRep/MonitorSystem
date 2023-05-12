#pragma once
#include "utils/CommonHeader.h"
#include "utils/CommandLine.h"

class ServerCommandLine : public CommandLine
{
public:
  ServerCommandLine();
  virtual ~ServerCommandLine();

  bool parse(const CommandLineArgs* cmdArgs);
  bool showHelp();
};

