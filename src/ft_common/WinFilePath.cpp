#include "WinFilePath.h"

WinFilePath::WinFilePath()
  : m_parentPathIsRoot(false)
{
}

WinFilePath::WinFilePath(const TCHAR* string)
  : m_parentPathIsRoot(false)
{
  setString(string);
}

WinFilePath::WinFilePath(const WinFilePath& stringBuffer)
  : m_parentPathIsRoot(false)
{
  setString(stringBuffer.getString());
}

WinFilePath::~WinFilePath()
{
}

bool WinFilePath::parentPathIsRoot()
{
  return m_parentPathIsRoot;
}

void WinFilePath::setString(const TCHAR* string)
{
  StringStorage str(string);
  if (!str.isEmpty()) {
    if (str.findLast('/') == 0) {
      m_parentPathIsRoot = true;
    }
    str.replaceChar(_T('/'), _T('\\'));
    if (str.beginsWith(_T('\\'))) {
      str.remove(0, 1);
    }

    if (str.endsWith(_T('\\'))) {
      str.truncate(1);
    }
  }
  StringStorage::setString(str.getString());
}

