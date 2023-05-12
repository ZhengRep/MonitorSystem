#pragma once
#include "Transport.h"
#include "network/socket/SocketIPv4.h"
#include "win_system/Pipe.h"
#include "win_system/PipeClient.h"
#include "win_system/PipeServer.h"

class TransportFactory
{
public:
  static Transport* createSocketClientTransport(const TCHAR* connectHost,
    unsigned int connectPort) throw(SocketException);
  static Transport* createSocketServerTransport(const TCHAR* bindHost,
    unsigned int bindPort) throw(SocketException);
  static Transport* createPipeClientTransport(const TCHAR* name) throw(Exception);
  static Transport* createPipeServerTransport(const TCHAR* name) throw(Exception);

private:
  TransportFactory() { };
};

