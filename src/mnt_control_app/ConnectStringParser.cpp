#include "ConnectStringParser.h"
#include "Utils/StringParser.h"

ConnectStringParser::ConnectStringParser()
{
}

ConnectStringParser::~ConnectStringParser()
{
}

bool ConnectStringParser::parse(const TCHAR* connectString, 
  StringStorage* connectHost, unsigned short* connectPort)
{
  StringStorage connStrStorage(connectString);

  size_t len = 2;

  StringStorage splitted[2];

  if (!connStrStorage.split(_T(":"), splitted, &len)) {
    return false;
  }

  if (len != 2) {
    return false;
  }

  int port = 0;

  if (!StringParser::parseInt(splitted[1].getString(), &port)) {
    return false;
  }

  if (port <= 0 || port > 65535) {
    return false;
  }

  if (connectHost != 0) {
    connectHost->setString(splitted[0].getString());
  }

  if (connectPort != 0) {
    *connectPort = (unsigned short)port;
  }

  return true;
}
