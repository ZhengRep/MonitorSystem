#pragma once
#include "Utils/CommonHeader.h"
#include "Utils/CommandLine.h"

class ServerCommandLine : public CommandLine
{
public:
  ServerCommandLine();
  virtual ~ServerCommandLine();

  bool parse(const CommandLineArgs* cmdArgs);
  bool showHelp();
};

