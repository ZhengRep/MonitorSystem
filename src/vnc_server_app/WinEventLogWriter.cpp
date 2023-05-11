#include "WinEventLogWriter.h"

#define MSG_WARNING_MESSAGE 0x00000001
#define MSG_INFO_MESSAGE    0x00000002
#define MSG_ERROR_MESSAGE    0x00000003

WinEventLogWriter::WinEventLogWriter(LogWriter* log)
  : m_sysLog(log)
{
}

WinEventLogWriter::~WinEventLogWriter()
{
}

void WinEventLogWriter::enable()
{
  m_sysLog.enable();
}

void WinEventLogWriter::onSuccAuth(const StringStorage* ip)
{
  m_sysLog.reportInfo(MSG_INFO_MESSAGE,
    _T("Authentication passed by %s"),
    ip->getString());
}

void WinEventLogWriter::onAuthFailed(const StringStorage* ip)
{
  m_sysLog.reportWarning(MSG_WARNING_MESSAGE,
    _T("Authentication failed from %s"),
    ip->getString());
}

void WinEventLogWriter::onDisconnect(const StringStorage* message)
{
  m_sysLog.reportInfo(MSG_INFO_MESSAGE, _T("%s"), message->getString());
}

void WinEventLogWriter::onCrash(const StringStorage* dumpPath)
{
  m_sysLog.reportError(MSG_ERROR_MESSAGE,
    _T("Application crashed. Debug information has been saved to %s"),
    dumpPath->getString());
}

void WinEventLogWriter::onSuccServiceStart()
{
  m_sysLog.reportInfo(MSG_INFO_MESSAGE,
    _T("Service has been started successfully"));
}

void WinEventLogWriter::onFailedServiceStart(const StringStorage* reason)
{
  m_sysLog.reportError(MSG_ERROR_MESSAGE,
    _T("Service has been terminated for the following reason: %s"),
    reason->getString());
}

void WinEventLogWriter::onServiceStop()
{
  m_sysLog.reportInfo(MSG_INFO_MESSAGE, _T("Service has been stopped"));
}
