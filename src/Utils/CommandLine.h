#pragma once
#include "CommonHeader.h"
#include "CommandLineArgs.h"
#include "KeyContainer.h"

enum UseArgument {
  NO_ARG,
  NEEDS_ARG
};

struct CommandLineFormat
{
  const TCHAR* keyName;
  UseArgument useArg;
};

class CommandLine
{
public:
  CommandLine();
  ~CommandLine();
  bool parse(const CommandLineFormat* format, int formatSize,
    const CommandLineArgs* cmdArgs);
  bool optionSpecified(const TCHAR* key, StringStorage* arg = 0) const;
  bool getOption(int index, StringStorage* key, StringStorage* arg = 0) const;

protected:
  bool matchKey(const TCHAR* keyTemplate, StringStorage* key);
  bool removeKeyPrefix(StringStorage* key);

  std::vector<KeyContainer> m_foundKeys;
};

