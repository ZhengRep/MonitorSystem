#include "RfbServer.h"
#include "server_config_lib/ServerConfig.h"
#include "server_config_lib/Configurator.h"

RfbServer::RfbServer(const TCHAR* bindHost, unsigned short bindPort, RfbClientManager* clientManager, bool lockAddr, LogWriter* log, const Rect* viewPort) throw(Exception)
  : TcpServer(bindHost, bindPort, false, lockAddr),
    m_clientManager(clientManager),
    m_log(log)
{
  if (viewPort != 0) {
    m_viewPort.setArbitraryRect(viewPort);
  }

  TcpServer::start();

  if (viewPort == 0) {
    m_log->message(_T("Rfb server started at %s:%d"), bindHost, (int)bindPort);
  } else {
    m_log->message(_T("Rfb server started at %s:%d with [%d %d %d %d] view port specified"),
      bindHost, (int)bindPort,
      viewPort->left, viewPort->right, viewPort->top, viewPort->bottom);
  }
}

RfbServer::~RfbServer()
{
  m_log->message(_T("Rfb server at %s:%d stopped"), getBindHost(), (int)getBindPort());
}

void RfbServer::onAcceptConnection(SocketIPv4* socket)
{
  try {
    // Get incoming connection address and convert it to string.
    SocketAddressIPv4 peerAddr;
    socket->getPeerAddr(&peerAddr);
    StringStorage peerIpString;
    peerAddr.toString(&peerIpString);

    m_log->message(_T("Incoming rfb connection from %s to port %u"), peerIpString.getString(), peerAddr.getPort());

    struct sockaddr_in addr_in = peerAddr.getSockAddr();

    // Check access control rules for the IP address of the peer.
    ServerConfig* config = Configurator::getInstance()->getServerConfig();
    IpAccessRule::ActionType action = config->getActionByAddress((unsigned long)addr_in.sin_addr.S_un.S_addr);

    if (action == IpAccessRule::ACTION_TYPE_DENY) {
      m_log->message(_T("Connection rejected due to access control rules"));
      delete socket;
      return;
    }

    // Access granted, add new RFB client. One more check will follow later in
    // RfbClientManager::onCheckAccessControl().

    socket->enableNaggleAlgorithm(false);

    m_clientManager->addNewConnection(socket, &m_viewPort, false, false);

  } catch (Exception& ex) {
    m_log->error(_T("Failed to process incoming rfb connection with following reason: \"%s\""), ex.getMessage());
  }
}
