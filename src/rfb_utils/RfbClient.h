#pragma once
#include "network/socket/SocketIPv4.h"
#include "win_system/WindowsEvent.h"
#include "thread/Thread.h"
#include "network/RfbOutputGate.h"
#include "desktop/Desktop.h"
#include "fb_update_sender/UpdateSender.h"
#include "log_writer/LogWriter.h"
#include "RfbDispatcher.h"
#include "ClipboardExchange.h"
#include "ClientInputHandler.h"
#include "ClientInputEventListener.h"
#include "ClientTerminationListener.h"
#include "mnt_server_app/NewConnectionEvents.h"
#include "utils/DemandTimer.h"

class ClientAuthListener;

enum ClientState
{
  IN_NONAUTH,
  IN_AUTH,
  IN_NORMAL_PHASE,
  IN_PENDING_TO_REMOVE,
  IN_READY_TO_REMOVE
};

class RfbClient: public Thread, ClientInputEventListener, private SenderControlInformationInterface
{
public:
  RfbClient(NewConnectionEvents* newConnectionEvents, SocketIPv4* socket,
    ClientTerminationListener* extTermListener, ClientAuthListener* extAuthListener, bool viewOnly,
    bool isOutgoing, unsigned int id, const ViewPortState* constViewPort, const ViewPortState* dynViewPort,
    int idleTimeout, LogWriter* log);
  virtual ~RfbClient();

  void disconnect();

  ClientState getClientState();
  unsigned int getId() const;
  void getPeerHost(StringStorage* address);
  void getLocalIpAddress(StringStorage* address);
  void getSocketAddr(SocketAddressIPv4* addr) const;
  bool isOutgoing() const;

  bool getSharedFlag() const { return m_shared; }
  bool getViewOnlyAuth() const { return m_viewOnlyAuth; }

  void setViewOnlyFlag(bool value);

  // Changes current dynViewPort value by new.
  void changeDynViewPort(const ViewPortState* dynViewPort);

  bool clientIsReady() const { return m_updateSender->clientIsReady(); }
  void sendUpdate(const UpdateContainer* updateContainer, const CursorShape* cursorShape);
  void sendClipboard(const StringStorage* newClipboard);

protected:
  virtual void execute();
  virtual void onTerminate();

private:
  void notifyAbStateChanging(ClientState state);
  virtual void onKeyboardEvent(UINT32 keySym, bool down);
  virtual void onMouseEvent(UINT16 x, UINT16 y, UINT8 buttonMask);

  void setClientState(ClientState newState);

  Rect getViewPortRect(const Dimension* fbDimension);
  virtual void onGetViewPort(Rect* viewRect, bool* shareApp, Region* shareAppRegion);
  void getViewPortInfo(const Dimension* fbDimension, Rect* resultRect, bool* shareApp, Region* shareAppRegion);

  ClientState m_clientState;
  bool m_isMarkedOk;
  LocalMutex m_clientStateMut;
  ClientTerminationListener* m_extTermListener;
  WindowsEvent m_connClosingEvent;

  SocketIPv4* m_socket;
  ClientAuthListener* m_extAuthListener;

  ViewPort m_constViewPort;
  ViewPort m_dynamicViewPort;
  LocalMutex m_viewPortMutex;

  UpdateSender* m_updateSender;
  ClipboardExchange* m_clipboardExchange;
  ClientInputHandler* m_clientInputHandler;
  Desktop* m_desktop;


  bool m_viewOnly;
  bool m_isOutgoing;
  bool m_viewOnlyAuth;
  bool m_shared;

  LogWriter* m_log;

  // Information
  unsigned int m_id;
  NewConnectionEvents* m_newConnectionEvents;
  // This timer sets by IdleTimeout value from server config 
  // and resets on mouse or keyboard event
  DemandTimer m_idleTimer;
  int m_idleTimeout;
};

