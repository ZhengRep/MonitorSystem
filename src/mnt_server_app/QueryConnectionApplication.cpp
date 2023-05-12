#include "QueryConnectionApplication.h"
#include "QueryConnectionCommandLine.h"
#include "win_system/WinCommandLineArgs.h"
#include "QueryConnectionDialog.h"

QueryConnectionApplication::QueryConnectionApplication(HINSTANCE hInstance, const TCHAR* windowClassName, const TCHAR* cmdLine)
  : LocalWindowsApplication(hInstance, windowClassName),
     m_cmdLine(cmdLine)
{
}

QueryConnectionApplication::~QueryConnectionApplication()
{
}

int QueryConnectionApplication::run()
{
  QueryConnectionCommandLine parser;

  try {
    WinCommandLineArgs cmdArgs(m_cmdLine.getString());
    parser.parse(&cmdArgs);
  } catch (Exception&) {
    //MntServerHelp::showUsage();
    return 0;
  }

  StringStorage peerAddress;

  parser.getPeerAddress(&peerAddress);

  DWORD queryTimeout = parser.isTimeoutSpecified() ? parser.getTimeout() : 30;

  QueryConnectionDialog dialog(peerAddress.getString(), parser.isDefaultActionAccept(), queryTimeout);

  return dialog.showModal();
}

int QueryConnectionApplication::execute(const TCHAR* peerAddr, bool acceptByDefault, DWORD timeOutSec) throw(Exception)
{
    return 0;
}
