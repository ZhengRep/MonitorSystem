#pragma once
#include "Utils/CommandLine.h"
#include "Utils/CommandLineFormatException.h"
#include "region/Rect.h"

class ControlCommandLine : private CommandLine
{
public:
  static const TCHAR CONFIG_APPLICATION[];
  static const TCHAR CONFIG_SERVICE[];

  static const TCHAR CONTROL_SERVICE[];
  static const TCHAR CONTROL_APPLICATION[];
  static const TCHAR PASSWORD_FILE[];
  static const TCHAR CONFIG_RELOAD[];
  static const TCHAR DISCONNECT_ALL[];
  static const TCHAR CONNECT[];
  static const TCHAR SHUTDOWN[];
  static const TCHAR SHARE_PRIMARY[];
  static const TCHAR SHARE_RECT[];
  static const TCHAR SHARE_DISPLAY[];
  static const TCHAR SHARE_WINDOW[];
  static const TCHAR SHARE_FULL[];
  static const TCHAR SHARE_APP[];

  static const TCHAR SET_CONTROL_PASSWORD[];
  static const TCHAR SET_PRIMARY_MNT_PASSWORD[];
  static const TCHAR CHECK_SERVICE_PASSWORDS[];

  static const TCHAR SLAVE_MODE[];
  static const TCHAR DONT_ELEVATE[];

public:
  ControlCommandLine();
  virtual ~ControlCommandLine();

  void parse(const CommandLineArgs* cmdArgs) throw(CommandLineFormatException);

  void getPasswordFile(StringStorage* passwordFile) const;
  bool hasPasswordFile();
  bool hasReloadFlag();
  bool hasKillAllFlag();
  bool hasConnectFlag();
  void getConnectHostName(StringStorage* hostName) const;
  bool hasShutdownFlag();
  bool hasSetMntPasswordFlag();
  bool hasSetControlPasswordFlag();
  bool hasCheckServicePasswords();
  bool hasConfigAppFlag();
  bool hasConfigServiceFlag();
  bool hasControlServiceFlag();
  bool hasControlAppFlag();
  bool hasDontElevateFlag();
  bool isSlave();

  bool hasSharePrimaryFlag();
  bool hasShareRect();
  bool hasShareDisplay();
  bool hasShareWindow();
  bool hasShareFull();
  bool hasShareApp();
  unsigned char getShareDisplayNumber();
  void getShareWindowName(StringStorage* out);
  Rect getShareRect();
  unsigned int getSharedAppProcessId();

  const TCHAR* getPrimaryVncPassword() const;
  const TCHAR* getControlPassword() const;

  bool isCommandSpecified();

private:
  void parseRectCoordinates(const StringStorage* strCoord);
  void parseDisplayNumber(const StringStorage* strDispNumber);
  void parseProcessId(const StringStorage* str);

  StringStorage m_mntPassword;
  StringStorage m_controlPassword;

  StringStorage m_connectHostName;
  StringStorage m_dispatcherSpec;
  StringStorage m_passwordFile;

  Rect m_shareRect;
  unsigned char m_displayNumber;
  StringStorage m_windowHeaderName;
  unsigned int m_sharedAppProcessId;
};

