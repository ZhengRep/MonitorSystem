#pragma once
#include "utils/CommandLine.h"
#include "utils/Exception.h"

/**
 * Desktop server command line parser.
 * @valid command line is '-desktopserver -logdir pathToLogDir' where
 * pathToLogDir arg is full path to directory where desktop server log
 * will be created.
 */
class DesktopServerCommandLine : private CommandLine
{
public:
  DesktopServerCommandLine();
  virtual ~DesktopServerCommandLine();

  virtual void parse(const CommandLineArgs* cmdArgs) throw(Exception);

  void getLogDir(StringStorage* logDir);
  int getLogLevel();


  void getSharedMemName(StringStorage* shMemName);

public:
  const static TCHAR DESKTOP_SERVER_KEY[];

private:
  const static TCHAR LOG_DIR_KEY[];
  const static TCHAR LOG_LEVEL_KEY[];
  const static TCHAR SHARED_MEMORY_NAME_KEY[];
};

