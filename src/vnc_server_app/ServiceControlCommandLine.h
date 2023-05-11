#pragma once
#include "Utils/CommandLine.h"
#include "Utils/Exception.h"

/**
 * Service control application command line parser.
 * Supported keys are:
 *  -install, -reinstall, -remove, -start, -stop.
 */
class ServiceControlCommandLine : private CommandLine
{
public:
  static const TCHAR APPLICATION_KEY[];
  static const TCHAR INSTALL_SERVICE[];
  static const TCHAR REMOVE_SERVICE[];
  static const TCHAR REINSTALL_SERVICE[];
  static const TCHAR START_SERVICE[];
  static const TCHAR STOP_SERVICE[];
  static const TCHAR SILENT[];
  static const TCHAR DONT_ELEVATE[];

public:
  ServiceControlCommandLine();
  virtual ~ServiceControlCommandLine();
  void parse(const CommandLineArgs* cmdArgs) throw(Exception);
  bool keySpecified(const TCHAR* key) const;

  bool installationRequested() const;
  bool removalRequested() const;
  bool reinstallRequested() const;
  bool startRequested() const;
  bool stopRequested() const;

  bool beSilent() const;
  bool dontElevate() const;
};

