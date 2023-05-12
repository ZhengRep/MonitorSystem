#include "ControlCommandLine.h"

#include "utils/CommonHeader.h"
#include "utils/CommandLine.h"
#include "region/RectSerializer.h"
#include "utils/StringParser.h"
#include "ConnectStringParser.h"

const TCHAR ControlCommandLine::SET_CONTROL_PASSWORD[] = _T("-setservicecontrolpass");
const TCHAR ControlCommandLine::SET_PRIMARY_MNT_PASSWORD[] = _T("-setservicevncpass");
const TCHAR ControlCommandLine::CHECK_SERVICE_PASSWORDS[] = _T("-checkservicepasswords");

const TCHAR ControlCommandLine::CONTROL_SERVICE[] = _T("-controlservice");
const TCHAR ControlCommandLine::CONTROL_APPLICATION[] = _T("-controlapp");
const TCHAR ControlCommandLine::PASSWORD_FILE[] = _T("-passfile");
const TCHAR ControlCommandLine::CONFIG_RELOAD[] = _T("-reload");
const TCHAR ControlCommandLine::DISCONNECT_ALL[] = _T("-disconnectall");
const TCHAR ControlCommandLine::CONNECT[] = _T("-connect");
const TCHAR ControlCommandLine::SHUTDOWN[] = _T("-shutdown");
const TCHAR ControlCommandLine::SHARE_PRIMARY[] = _T("-shareprimary");
const TCHAR ControlCommandLine::SHARE_RECT[] = _T("-sharerect");
const TCHAR ControlCommandLine::SHARE_DISPLAY[] = _T("-sharedisplay");
const TCHAR ControlCommandLine::SHARE_WINDOW[] = _T("-sharewindow");
const TCHAR ControlCommandLine::SHARE_FULL[] = _T("-sharefull");
const TCHAR ControlCommandLine::SHARE_APP[] = _T("-shareapp");

const TCHAR ControlCommandLine::CONFIG_APPLICATION[] = _T("-configapp");
const TCHAR ControlCommandLine::CONFIG_SERVICE[] = _T("-configservice");

const TCHAR ControlCommandLine::SLAVE_MODE[] = _T("-slave");

const TCHAR ControlCommandLine::DONT_ELEVATE[] = _T("-dontelevate");

ControlCommandLine::ControlCommandLine()
  : m_displayNumber(0),
  m_sharedAppProcessId(0)
{
}

ControlCommandLine::~ControlCommandLine()
{
}

void ControlCommandLine::parse(const CommandLineArgs* cmdArgs)
{
  CommandLineFormat fmt[] = {
    { PASSWORD_FILE, NEEDS_ARG },
    { CONFIG_RELOAD, NO_ARG },
    { DISCONNECT_ALL, NO_ARG },
    { CONNECT, NEEDS_ARG },
    { SHUTDOWN, NO_ARG },
    { SET_PRIMARY_MNT_PASSWORD, NEEDS_ARG },
    { SET_CONTROL_PASSWORD, NEEDS_ARG },
    { CHECK_SERVICE_PASSWORDS, NO_ARG },
    { SHARE_PRIMARY, NO_ARG },
    { SHARE_RECT, NEEDS_ARG },
    { SHARE_DISPLAY, NEEDS_ARG },
    { SHARE_WINDOW, NEEDS_ARG },
    { SHARE_FULL, NO_ARG },
    { SHARE_APP, NEEDS_ARG },
    { CONTROL_SERVICE, NO_ARG },
    { CONTROL_APPLICATION, NO_ARG },
    { CONFIG_APPLICATION, NO_ARG },
    { CONFIG_SERVICE, NO_ARG },
    { SLAVE_MODE, NO_ARG },
    { DONT_ELEVATE, NO_ARG }
  };

  if (!CommandLine::parse(fmt, sizeof(fmt) / sizeof(CommandLineFormat), cmdArgs)) {
    throw CommandLineFormatException();
  }

  if (hasConfigServiceFlag() && ((int)m_foundKeys.size() > (optionSpecified(DONT_ELEVATE) ? 2 : 1))) {
    throw CommandLineFormatException();
  }

  if (hasConfigAppFlag() && m_foundKeys.size() > 1) {
    throw CommandLineFormatException();
  }

  bool hasPassFile = hasPasswordFile();
  if (hasPassFile) {
    optionSpecified(PASSWORD_FILE, &m_passwordFile);
  }

  if (hasShareRect()) {
    StringStorage strRect;
    optionSpecified(SHARE_RECT, &strRect);
    parseRectCoordinates(&strRect);
  }

  if (hasShareDisplay()) {
    StringStorage strDisplayNumber;
    optionSpecified(SHARE_DISPLAY, &strDisplayNumber);
    parseDisplayNumber(&strDisplayNumber);
  }

  if (hasShareWindow()) {
    optionSpecified(SHARE_WINDOW, &m_windowHeaderName);
  }

  if (hasShareApp()) {
    StringStorage strAppProcId;
    optionSpecified(SHARE_APP, &strAppProcId);
    parseProcessId(&strAppProcId);
  }

  if (hasKillAllFlag() && hasReloadFlag()) {
    throw CommandLineFormatException();
  }

  if (hasConnectFlag()) {
    optionSpecified(CONNECT, &m_connectHostName);
  }

  if ((hasSetMntPasswordFlag() || hasSetControlPasswordFlag()) && m_foundKeys.size() > 1) {
    throw CommandLineFormatException();
  } else {
    optionSpecified(SET_CONTROL_PASSWORD, &m_controlPassword);
    optionSpecified(SET_PRIMARY_MNT_PASSWORD, &m_mntPassword);
  }

  if ((hasControlServiceFlag() || hasControlAppFlag()) && (isSlave()) && (m_foundKeys.size() > 2)) {
    throw CommandLineFormatException();
  }

  bool hasNotSlaveControl = (hasControlServiceFlag() || hasControlAppFlag()) && !isSlave();
  if ((hasNotSlaveControl && !hasPassFile && m_foundKeys.size() > 2) ||
    (hasNotSlaveControl && hasPassFile && m_foundKeys.size() != 3)) {
    throw CommandLineFormatException();
  }

  if (m_foundKeys.size() == 0) {
    throw CommandLineFormatException();
  }
}

void ControlCommandLine::getPasswordFile(StringStorage* passwordFile) const
{
  *passwordFile = m_passwordFile;
}

bool ControlCommandLine::hasPasswordFile()
{
  return optionSpecified(PASSWORD_FILE);
}

bool ControlCommandLine::hasReloadFlag()
{
  return optionSpecified(CONFIG_RELOAD);
}

bool ControlCommandLine::hasKillAllFlag()
{
  return optionSpecified(DISCONNECT_ALL);
}

bool ControlCommandLine::hasConnectFlag()
{
  return optionSpecified(CONNECT);
}

void ControlCommandLine::getConnectHostName(StringStorage* hostName) const
{
  *hostName = m_connectHostName;
}

bool ControlCommandLine::hasShutdownFlag()
{
  return optionSpecified(SHUTDOWN);
}

bool ControlCommandLine::hasSetMntPasswordFlag()
{
  return optionSpecified(SET_PRIMARY_MNT_PASSWORD);
}

bool ControlCommandLine::hasSetControlPasswordFlag()
{
  return optionSpecified(SET_CONTROL_PASSWORD);
}

bool ControlCommandLine::hasConfigAppFlag()
{
  return optionSpecified(CONFIG_APPLICATION);
}

bool ControlCommandLine::hasConfigServiceFlag()
{
  return optionSpecified(CONFIG_SERVICE);
}

bool ControlCommandLine::hasDontElevateFlag()
{
  return optionSpecified(DONT_ELEVATE);
}

bool ControlCommandLine::hasControlServiceFlag()
{
  return optionSpecified(CONTROL_SERVICE);
}

bool ControlCommandLine::hasControlAppFlag()
{
  return optionSpecified(CONTROL_APPLICATION);
}

bool ControlCommandLine::isSlave()
{
  return optionSpecified(SLAVE_MODE);
}

bool ControlCommandLine::hasSharePrimaryFlag()
{
  return optionSpecified(SHARE_PRIMARY);
}

bool ControlCommandLine::hasShareRect()
{
  return optionSpecified(SHARE_RECT);
}

bool ControlCommandLine::hasShareDisplay()
{
  return optionSpecified(SHARE_DISPLAY);
}

bool ControlCommandLine::hasShareWindow()
{
  return optionSpecified(SHARE_WINDOW);
}

bool ControlCommandLine::hasShareFull()
{
  return optionSpecified(SHARE_FULL);
}

bool ControlCommandLine::hasShareApp()
{
  return optionSpecified(SHARE_APP);
}

unsigned char ControlCommandLine::getShareDisplayNumber()
{
  return m_displayNumber;
}

void ControlCommandLine::getShareWindowName(StringStorage* out)
{
  *out = m_windowHeaderName;
}

Rect ControlCommandLine::getShareRect()
{
  return m_shareRect;
}

unsigned int ControlCommandLine::getSharedAppProcessId()
{
  return m_sharedAppProcessId;
}

bool ControlCommandLine::hasCheckServicePasswords()
{
  return optionSpecified(CHECK_SERVICE_PASSWORDS);
}

const TCHAR* ControlCommandLine::getPrimaryVncPassword() const
{
  return m_mntPassword.getString();
}

const TCHAR* ControlCommandLine::getControlPassword() const
{
  return m_controlPassword.getString();
}

bool ControlCommandLine::isCommandSpecified()
{
  return hasKillAllFlag() || hasReloadFlag() || hasSetControlPasswordFlag() ||
    hasSetMntPasswordFlag() || hasConnectFlag() || hasShutdownFlag() ||
    hasSharePrimaryFlag() || hasShareDisplay() || hasShareWindow() ||
    hasShareRect() || hasShareFull() || hasShareApp();
}

void ControlCommandLine::parseRectCoordinates(const StringStorage* strCoord)
{
  m_shareRect = RectSerializer::toRect(strCoord);
}

void ControlCommandLine::parseDisplayNumber(const StringStorage* strDispNumber)
{
  if (!StringParser::parseByte(strDispNumber->getString(),
    &m_displayNumber)) {
    StringStorage errMess;
    errMess.format(_T("Can't parse the %s argument to a display number"),
      strDispNumber->getString());
    throw Exception(errMess.getString());
  }
}

void ControlCommandLine::parseProcessId(const StringStorage* str)
{
  if (!StringParser::parseUInt(str->getString(),
    &m_sharedAppProcessId)) {
    StringStorage errMess;
    errMess.format(_T("Can't parse the %s argument to a process id"),
      str->getString());
    throw Exception(errMess.getString());
  }
}
