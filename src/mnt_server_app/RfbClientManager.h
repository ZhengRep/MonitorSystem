#pragma once
#include "utils/ListenerContainer.h"
#include "rfb_utils/RfbClient.h"
#include "thread/AutoLock.h"
#include "thread/Thread.h"
#include "win_system/WindowsEvent.h"
#include "desktop/Desktop.h"
#include "desktop/DesktopFactory.h"
#include "log_writer/LogWriter.h"

//Listener
#include "RfbClientManagerEventListener.h"
#include "rfb_utils/ClientTerminationListener.h"
#include "desktop/ClipboardListener.h"
#include "desktop/AbnormDeskTermListener.h"
#include "desktop/UpdateListener.h"
#include "rfb_utils/ClientAuthListener.h"
#include "mnt_control_app/RfbClientInfo.h"
#include "NewConnectionEvents.h"

typedef std::list<RfbClient*> ClientList;
typedef std::list<RfbClient*>::iterator ClientListIter;

struct BanProp
{
  unsigned int count;
  DateTime banLastTime;
};

typedef std::map<StringStorage, BanProp> BanList;
typedef BanList::iterator BanListIter;

class RfbClientManager
  : public ClientTerminationListener,
    public ClipboardListener,
    public UpdateSendingListener,
    public ClientAuthListener,
    public AbnormDeskTermListener,
    public ListenerContainer<RfbClientManagerEventListener*>
{
public:
  // FIXME: parameter is not used.
  RfbClientManager(const TCHAR* serverName, NewConnectionEvents* newConnectionEvents,
    LogWriter* log, DesktopFactory* desktopFactory);
  virtual ~RfbClientManager();

  void getClientsInfo(RfbClientInfoList* list);
  virtual void disconnectAllClients();
  virtual void disconnectNonAuthClients();
  virtual void disconnectAuthClients();

  void setDynViewPort(const ViewPortState* dynViewPort);
  void addNewConnection(SocketIPv4* socket, ViewPortState* constViewPort,
    bool viewOnly, bool isOutgoing);
  BanList getBanList() { AutoLock al(&m_banListMutex); return m_banList; };
  StringStorage getBanListString();

protected:
  // Listen functions
  virtual void onClientTerminate();
  virtual Desktop* onClientAuth(RfbClient* client);
  virtual bool onCheckForBan(RfbClient* client);

  virtual void onAuthFailed(RfbClient* client);
  virtual void onCheckAccessControl(RfbClient* client) throw(AuthException);
  virtual void onClipboardUpdate(const StringStorage* newClipboard);
  virtual void onSendUpdate(const UpdateContainer* updateContainer,
    const CursorShape* cursorShape);
  virtual bool isReadyToSend();
  virtual void onAbnormalDesktopTerminate();

  void waitUntilAllClientAreBeenDestroyed();

private:
  void validateClientList();
  bool checkForBan(const StringStorage* ip);
  void updateIpInBan(const StringStorage* ip, bool success);


  ClientList m_nonAuthClientList;
  ClientList m_clientList;
  LocalMutex m_clientListLocker;

  ViewPortState m_dynViewPort;

  BanList m_banList;
  WindowsEvent m_banTimer;
  LocalMutex m_banListMutex;

  WindowsEvent m_listUnderflowingEvent;
  // Creating and destroying this object must be with the locked
  // m_clientListLocker
  Desktop* m_desktop;
  DesktopFactory* m_desktopFactory;

  // Inforamtion
  unsigned int m_nextClientId;

  NewConnectionEvents* m_newConnectionEvents;

  LogWriter* m_log;
};

