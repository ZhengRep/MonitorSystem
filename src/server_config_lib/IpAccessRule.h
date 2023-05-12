#pragma once
#include "utils/StringStorage.h"

//Class contains information about
//host access (allow, deny hosts etc)

class IpAccessRule
{
public:
  //Host access type
  enum ActionType {
    ACTION_TYPE_ALLOW = 0,
    ACTION_TYPE_DENY = 1,
    ACTION_TYPE_QUERY = 2
  };

public:
  IpAccessRule();
  virtual ~IpAccessRule();
public:
  void toString(StringStorage* output) const;
  static bool parse(const TCHAR* string, IpAccessRule* rule);
  static bool parseIp(const TCHAR* string, IpAccessRule* rule);
  static bool parseIpRange(const TCHAR* string, IpAccessRule* rule);
  static bool parseSubnet(const TCHAR* string, IpAccessRule* rule);

  ActionType getAction() const {
    return m_action;
  }

  void setAction(ActionType value) {
    m_action = value;
  }

  void getFirstIp(StringStorage* firstIp) const;
  void getLastIp(StringStorage* lastIp) const;

  void setFirstIp(const TCHAR* firstIp);
  void setLastIp(const TCHAR* lastIp);
  bool isEqualTo(IpAccessRule* other) const;

  bool isIncludingAddress(unsigned long ip) const;
  static bool isIpAddressStringValid(const TCHAR* string);

  //
  // Return values:
  // 0  - equal
  // -1 - ip1 < ip2
  // 1  - ip1 > ip2
  //

  static int compareIp(unsigned long ip1, unsigned long ip2);
protected:
  static bool tryParseIPPart(const TCHAR* string);
  static void getIpRange(const TCHAR* ip, const TCHAR* netmask, StringStorage* firstIp, StringStorage* lastIp);

protected:
  ActionType m_action;
  StringStorage m_firstIp;
  StringStorage m_lastIp;
};

