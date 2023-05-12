#include "TransportFactory.h"
#include "SocketIPv4Transport.h"
#include "NamedPipeTransport.h"
#include "win_system/SecurityAttributes.h"

Transport* TransportFactory::createSocketClientTransport(const TCHAR* connectHost,
  unsigned int connectPort)
{
  SocketIPv4* socket = new SocketIPv4();

  try {
    socket->connect(connectHost, connectPort);
  } catch (SocketException&) {
    delete socket;
    throw;
  }

  return new SocketIPv4Transport(socket);
}

Transport* TransportFactory::createSocketServerTransport(const TCHAR* bindHost,
  unsigned int bindPort)
{
  SocketIPv4* socket = new SocketIPv4();

  try {
    socket->bind(bindHost, bindPort);
    socket->listen(15);
  } catch (SocketException&) {
    delete socket;
    throw;
  }

  return new SocketIPv4Transport(socket);
}

Transport* TransportFactory::createPipeClientTransport(const TCHAR* name)
{
  return new NamedPipeTransport(PipeClient::connect(name, 0));
}

Transport* TransportFactory::createPipeServerTransport(const TCHAR* name)
{
  SecurityAttributes* pipeSecurity = new SecurityAttributes();

  pipeSecurity->setInheritable();
  pipeSecurity->shareToAllUsers();

  return new NamedPipeTransport(new PipeServer(name, 0, pipeSecurity));
}
