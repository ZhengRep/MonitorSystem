#include "RfbClientManager.h"
#include "thread/ZombieKiller.h"
#include "server_config_lib/Configurator.h"
#include "utils/MemUsage.h"
#include "QueryConnectionApplication.h"

RfbClientManager::RfbClientManager(const TCHAR* serverName, NewConnectionEvents* newConnectionEvents, LogWriter* log, DesktopFactory* desktopFactory)
  : m_nextClientId(0),
  m_desktop(0),
  m_newConnectionEvents(newConnectionEvents),
  m_log(log),
  m_desktopFactory(desktopFactory)
{
  m_log->info(_T("Starting rfb client manager"));
}

RfbClientManager::~RfbClientManager()
{
  m_log->info(_T("~RfbClientManager() has been called"));
  disconnectAllClients();
  waitUntilAllClientAreBeenDestroyed();
  m_log->info(_T("~RfbClientManager() has been completed"));
}

void RfbClientManager::getClientsInfo(RfbClientInfoList* list)
{
  AutoLock al(&m_clientListLocker);

  for (ClientListIter it = m_clientList.begin(); it != m_clientList.end(); it++) {
    RfbClient* each = *it;
    if (each->getClientState() == IN_NORMAL_PHASE) {
      StringStorage peerHost;

      each->getPeerHost(&peerHost);

      list->push_back(RfbClientInfo(each->getId(), peerHost.getString()));
    }
  }
}

void RfbClientManager::disconnectAllClients()
{
  AutoLock al(&m_clientListLocker);
  disconnectNonAuthClients();
  disconnectAuthClients();
}

void RfbClientManager::disconnectNonAuthClients()
{
  AutoLock al(&m_clientListLocker);
  for (ClientListIter iter = m_nonAuthClientList.begin();
    iter != m_nonAuthClientList.end(); iter++) {
    (*iter)->disconnect();
  }
}

void RfbClientManager::disconnectAuthClients()
{
  AutoLock al(&m_clientListLocker);
  for (ClientListIter iter = m_clientList.begin();
    iter != m_clientList.end(); iter++) {
    (*iter)->disconnect();
  }
}

void RfbClientManager::setDynViewPort(const ViewPortState* dynViewPort)
{
  AutoLock al(&m_clientListLocker);
  m_dynViewPort = *dynViewPort;

  // Assign the dynViewPort value for all already run clients too.
  for (ClientListIter iter = m_clientList.begin();
    iter != m_clientList.end(); iter++) {
    (*iter)->changeDynViewPort(dynViewPort);
  }
  for (ClientListIter iter = m_nonAuthClientList.begin();
    iter != m_nonAuthClientList.end(); iter++) {
    (*iter)->changeDynViewPort(dynViewPort);
  }
}

void RfbClientManager::addNewConnection(SocketIPv4* socket, ViewPortState* constViewPort, bool viewOnly, bool isOutgoing)
{
  AutoLock al(&m_clientListLocker);

  ServerConfig* config = Configurator::getInstance()->getServerConfig();
  int timeout = 1000 * config->getIdleTimeout();

  m_log->error(_T("Set socket idle timeout, %d ms"), timeout);

  try {
    socket->setSocketOptions(SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    socket->setSocketOptions(SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
  } catch (SocketException) {
    m_log->error(_T("Can't set socket timeout, error: %d"), WSAGetLastError());
  }

  _ASSERT(constViewPort != 0);

  m_log->error(_T("Client #%d connected"), m_nextClientId);
  m_log->debug(_T("new client, process memory usage: %d "), MemUsage::getCurrentMemUsage());

  m_nonAuthClientList.push_back(new RfbClient(m_newConnectionEvents,
    socket, this, this, viewOnly,
    isOutgoing,
    m_nextClientId,
    constViewPort,
    &m_dynViewPort,
    timeout,
    m_log));
  m_nextClientId++;
}

StringStorage RfbClientManager::getBanListString()
{
  StringStorage str;
  for (BanListIter it = m_banList.begin(); it != m_banList.end(); it++) {
    StringStorage ip = (*it).first;
    StringStorage s;
    unsigned int count = (*it).second.count;
    DateTime lastTime = (*it).second.banLastTime;
    StringStorage time;
    lastTime.toString(&time);
    s.format(_T("IP: %s, count: %d, last time: %s\n"), ip.getString(), count, time.getString());
    str.appendString(s.getString());
  }
  return str;
}

void RfbClientManager::onClientTerminate()
{
  validateClientList();
}

Desktop* RfbClientManager::onClientAuth(RfbClient* client)
{
  // The client is now authenticated, so remove its IP from the ban list.
  StringStorage ip;
  client->getPeerHost(&ip);
  updateIpInBan(&ip, true);

  m_newConnectionEvents->onSuccAuth(&ip);

  AutoLock al(&m_clientListLocker);

  // Checking if this client is allowed to connect, depending on its "shared"
  // flag and the server's configuration.
  ServerConfig* servConf = Configurator::getInstance()->getServerConfig();
  bool isAlwaysShared = servConf->isAlwaysShared();
  bool isNeverShared = servConf->isNeverShared();

  bool isResultShared;
  if (isAlwaysShared) {
    isResultShared = true;
  } else if (isNeverShared) {
    isResultShared = false;
  } else {
    isResultShared = client->getSharedFlag();
  }

  // If the client wishes to have exclusive access then remove other clients.
  if (!isResultShared) {
    // Which client takes priority, existing or incoming?
    if (servConf->isDisconnectingExistingClients()) {
      // Incoming
      disconnectAuthClients();
    } else {
      // Existing
      if (!m_clientList.empty()) {
        throw Exception(_T("Cannot disconnect existing clients and therefore")
          _T(" the client will be disconected")); // Disconnect this client
      }
    }
  }

  // Removing the client from the non-authorized clients list.
  for (ClientListIter iter = m_nonAuthClientList.begin();
    iter != m_nonAuthClientList.end(); iter++) {
    RfbClient* clientOfList = *iter;
    if (clientOfList == client) {
      m_nonAuthClientList.erase(iter);
      break;
    }
  }

  // Adding to the authorized list.
  m_clientList.push_back(client);

  if (m_desktop == 0 && !m_clientList.empty()) {
    // Create WinDesktop and notify listeners that the first client has been
    // connected.
    m_desktop = m_desktopFactory->createDesktop(this, this, this, m_log);
    vector<RfbClientManagerEventListener*>::iterator iter;
    for (iter = m_listeners.begin(); iter != m_listeners.end(); iter++) {
      (*iter)->afterFirstClientConnect();
    }
  }
  return m_desktop;
}

bool RfbClientManager::onCheckForBan(RfbClient* client)
{
  StringStorage ip;
  client->getPeerHost(&ip);

  return checkForBan(&ip);
}

void RfbClientManager::onAuthFailed(RfbClient* client)
{
  StringStorage ip;
  client->getPeerHost(&ip);

  updateIpInBan(&ip, false);

  m_newConnectionEvents->onAuthFailed(&ip);
}

void RfbClientManager::onCheckAccessControl(RfbClient* client) throw(AuthException)
{
  SocketAddressIPv4 peerAddr;

  try {
    client->getSocketAddr(&peerAddr);
  } catch (...) {
    throw AuthException(_T("Failed to get IP address of the RFB client"));
  }

  struct sockaddr_in addr_in = peerAddr.getSockAddr();

  ServerConfig* config = Configurator::getInstance()->getServerConfig();

  IpAccessRule::ActionType action;

  if (!client->isOutgoing()) {
    action = config->getActionByAddress((unsigned long)addr_in.sin_addr.S_un.S_addr);
  } else {
    action = IpAccessRule::ACTION_TYPE_ALLOW;
  }

  // Promt user to know what to do with incmoing connection.

  if (action == IpAccessRule::ACTION_TYPE_QUERY) {
    StringStorage peerHost;

    peerAddr.toString(&peerHost);

    int queryRetVal = QueryConnectionApplication::execute(peerHost.getString(),
      config->isDefaultActionAccept(),
      config->getQueryTimeout());
    if (queryRetVal == 1) {
      throw AuthException(_T("Connection has been rejected"));
    }
  }
}

void RfbClientManager::onClipboardUpdate(const StringStorage* newClipboard)
{
  AutoLock al(&m_clientListLocker);
  for (ClientListIter iter = m_clientList.begin();
    iter != m_clientList.end(); iter++) {
    if ((*iter)->getClientState() == IN_NORMAL_PHASE) {
      (*iter)->sendClipboard(newClipboard);
    }
  }
}

void RfbClientManager::onSendUpdate(const UpdateContainer* updateContainer, const CursorShape* cursorShape)
{
  AutoLock al(&m_clientListLocker);
  for (ClientListIter iter = m_clientList.begin();
    iter != m_clientList.end(); iter++) {
    if ((*iter)->getClientState() == IN_NORMAL_PHASE) {
      (*iter)->sendUpdate(updateContainer, cursorShape);
    }
  }
}

bool RfbClientManager::isReadyToSend()
{
  AutoLock al(&m_clientListLocker);
  bool isReady = false;
  for (ClientListIter iter = m_clientList.begin();
    iter != m_clientList.end(); iter++) {
    if ((*iter)->getClientState() == IN_NORMAL_PHASE) {
      isReady = isReady || (*iter)->clientIsReady();
    }
  }
  return isReady;
}

void RfbClientManager::onAbnormalDesktopTerminate()
{
  m_log->error(_T("onAbnormalDesktopTerminate() called"));
  disconnectAllClients();
}

void RfbClientManager::waitUntilAllClientAreBeenDestroyed()
{
  while (true) {
    {
      AutoLock al(&m_clientListLocker);
      if (m_clientList.empty() && m_nonAuthClientList.empty()) {
        break;
      }
    }
    m_listUnderflowingEvent.waitForEvent();
  }
  ZombieKiller::getInstance()->killAllZombies();
}

void RfbClientManager::validateClientList()
{
  Desktop* objectToDestroy = 0;
  {
    AutoLock al(&m_clientListLocker);
    // If clients are in the IN_READY_TO_REMOVE phase, remove them from the
    // non-authorized clients list.
    ClientListIter iter = m_nonAuthClientList.begin();
    while (iter != m_nonAuthClientList.end()) {
      RfbClient* client = *iter;
      ClientState state = client->getClientState();
      if (state == IN_READY_TO_REMOVE) {
        iter = m_nonAuthClientList.erase(iter);
        ZombieKiller::getInstance()->addZombie(client);
      } else {
        iter++;
      }
    }
    // If clients are in the IN_READY_TO_REMOVE phase, remove them from the
    // authorized clients list.
    iter = m_clientList.begin();
    while (iter != m_clientList.end()) {
      RfbClient* client = *iter;
      ClientState state = client->getClientState();
      if (state == IN_READY_TO_REMOVE) {
        iter = m_clientList.erase(iter);
        ZombieKiller::getInstance()->addZombie(client);
      } else {
        iter++;
      }
    }

    if (m_desktop != 0 && m_clientList.empty()) {
      objectToDestroy = m_desktop;
      m_desktop = 0;
    }
  }
  if (objectToDestroy != 0) {
    delete objectToDestroy;
    vector<RfbClientManagerEventListener*>::iterator iter;
    for (iter = m_listeners.begin(); iter != m_listeners.end(); iter++) {
      (*iter)->afterLastClientDisconnect();
    }
  }

  AutoLock al(&m_clientListLocker);
  if (m_clientList.empty() && m_nonAuthClientList.empty()) {
    m_listUnderflowingEvent.notify();
  }
}

bool RfbClientManager::checkForBan(const StringStorage* ip)
{
  AutoLock al(&m_banListMutex);

  BanListIter it = m_banList.find(*ip);
  if (it != m_banList.end()) {
    unsigned int count = (*it).second.count;
    DateTime lastTime = (*it).second.banLastTime;
    DateTime now = DateTime::now();
    if (count > 13) count = 13;
    // about 1 hour max login rate after 14 unsuccessful logins
    // wait about 1 minute after 8 unsuccessful logins
    if ((now - lastTime).getTime() < 500 * (1 << count)) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void RfbClientManager::updateIpInBan(const StringStorage* ip, bool success)
{
  AutoLock al(&m_banListMutex);

  BanListIter it = m_banList.find(*ip);
  if (success) {
    if (it != m_banList.end()) {
      // Even if client is already banned!
      m_banList.erase(it);
    }
  } else {
    if (it != m_banList.end()) {
      // Increase ban count
      (*it).second.count += 1;
      (*it).second.banLastTime = DateTime::now();
    } else {
      // Add new element to ban list with ban count == 0
      BanProp banProp;
      banProp.banLastTime = DateTime::now();
      banProp.count = 0;
      m_banList[*ip] = banProp;
    }
  }
}
