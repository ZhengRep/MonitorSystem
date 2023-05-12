#pragma once
#include "utils/StringStorage.h"

class WinFilePath: public StringStorage
{
public:
  WinFilePath();
  WinFilePath(const TCHAR* string);
  WinFilePath(const WinFilePath& stringBuffer);

  ~WinFilePath();

  bool parentPathIsRoot();
  virtual void setString(const TCHAR* string);

private:
  bool m_parentPathIsRoot;
};

