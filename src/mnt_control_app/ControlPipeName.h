#pragma once
#include "Utils/CommonHeader.h"
#include "log_writer/LogWriter.h"

class ControlPipeName
{
public:
  static void createPipeName(bool forService, StringStorage* pipeName, LogWriter* log);
  ControlPipeName() { };
};

