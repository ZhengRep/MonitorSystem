#include "WinCommandLineArgs.h"
#include "utils/Exception.h"
#include "utils/UnicodeStringStorage.h"
#include "utils/Macros.h"

WinCommandLineArgs::WinCommandLineArgs(const TCHAR* cmdLineInWinFormat)
{
  UnicodeStringStorage uniCmdLine(&unmove(StringStorage(cmdLineInWinFormat)));
  size_t cmdLen = uniCmdLine.getLength();
  if (cmdLen > 0) {
    int nArgs;
    LPWSTR* argList = CommandLineToArgvW(uniCmdLine.getString(), &nArgs);
    if (argList == 0) {
      throw Exception(_T("Invalid command line"));
    }
    for (int i = 0; i < nArgs; i++) {
      UnicodeStringStorage uniArg(argList[i]);
      StringStorage arg;
      uniArg.toStringStorage(&arg);
      if (arg.getLength() > 0) {
        m_args.push_back(arg);
      }
    }

    LocalFree(argList);
  }
}

WinCommandLineArgs::~WinCommandLineArgs()
{
}
