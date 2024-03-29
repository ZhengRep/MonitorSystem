#include "CommandLine.h"

CommandLine::CommandLine()
{
}

CommandLine::~CommandLine()
{
}

bool CommandLine::parse(const CommandLineFormat* format, int formatSize, const CommandLineArgs* cmdArgs)
{
  std::vector<StringStorage> argContainer;
  std::vector<StringStorage>::iterator argIter;
  cmdArgs->getArgVector(&argContainer);

  bool result = true;
  for (argIter = argContainer.begin(); argIter != argContainer.end() && result;
    argIter++) {
    StringStorage* key = &(*argIter);
    if (!removeKeyPrefix(key)) {
      result = false;
      break;
    }

    key->toLowerCase();

    result = false;
    for (int i = 0; i < formatSize; i++) {
      if (matchKey(format[i].keyName, key)) {
        KeyContainer keyContainer;
        result = true;

        if (format[i].useArg == NEEDS_ARG) {
          argIter++;
          if (argIter == argContainer.end()) {
            return false;
          }
          keyContainer.isArgument = true;
          keyContainer.argument.setString((*argIter).getString());
        }
        keyContainer.key.setString(format[i].keyName);
        m_foundKeys.push_back(keyContainer);
      }
    }
  }

  return result;
}

bool CommandLine::optionSpecified(const TCHAR* key, StringStorage* arg) const
{
  bool found = false;
  std::vector<KeyContainer>::const_iterator iter;
  for (iter = m_foundKeys.begin(); iter != m_foundKeys.end(); iter++) {
    const KeyContainer* foundKey = &(*iter);
    if (_tcscmp(foundKey->key.getString(), key) == 0) {
      found = true;
      if (foundKey->isArgument && arg != 0) {
        arg->setString(foundKey->argument.getString());
      }
    }
  }
  return found;
}

bool CommandLine::getOption(int index, StringStorage* key, StringStorage* arg) const
{
  if (index < 0 || (size_t)index >= m_foundKeys.size()) {
    return false;
  }

  const KeyContainer* foundKey = &m_foundKeys[(size_t)index];

  key->setString(foundKey->key.getString());

  if (foundKey->isArgument && arg != 0) {
    arg->setString(foundKey->argument.getString());
  }

  return true;
}

bool CommandLine::matchKey(const TCHAR* keyTemplate, StringStorage* key)
{
  return key->isEqualTo(++keyTemplate);
}

bool CommandLine::removeKeyPrefix(StringStorage* key)
{
  try {
    key->remove(0, 1);
    return true;
  } catch (...) {
    return false;
  }
}
