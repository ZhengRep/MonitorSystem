#pragma once
#include "Utils/CommonHeader.h"
#include "DesktopServerWatcher.h"
#include "desktop_ipc/DesktopConfigClient.h"
#include "desktop_ipc/ReconnectingChannel.h"
#include "desktop_ipc/BlockingGate.h"
#include "desktop_ipc/GateKicker.h"
#include "desktop_ipc/DesktopSrvDispatcher.h"
#include "DesktopBaseImpl.h"
#include "log_writer/LogWriter.h"
#include "Utils/AnEventListener.h"
#include "desktop_ipc/ReconnectionListener.h"
#include "UpdateListener.h"

class DesktopClientImpl: public AnEventListener, public ReconnectionListener,
  public Thread, public DesktopBaseImpl
{
public:
  DesktopClientImpl(ClipboardListener* extClipListener, UpdateSendingListener* extUpdSendingListener,
    AbnormDeskTermListener* extDeskTermListener, LogWriter* log);
  virtual ~DesktopClientImpl();

protected:
  virtual void execute();
  virtual void onTerminate();

private:
  // Interface functions
  virtual void onAnObjectEvent();
  virtual void onReconnect(Channel* newChannelTo, Channel* newChannelFrom);

  void freeResource();
  void closeDesktopServerTransport();

  virtual bool isRemoteInputTempBlocked();
  virtual void applyNewConfiguration();

  //Inter process transport
  ReconnectingChannel* m_clToSrvChan;
  ReconnectingChannel* m_srvToClChan;
  BlockingGate* m_clToSrvGate;
  BlockingGate* m_srvToClGate;

  DesktopServerWatcher* m_deskServWatcher;
  DesktopSrvDispatcher* m_dispatcher;

  GateKicker* m_gateKicker;
  UserInput* m_userInputClient;
  DesktopConfigClient* m_deskConf;

  LogWriter* m_log;

}; 

