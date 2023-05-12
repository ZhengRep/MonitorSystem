#pragma once
#include "RfbClientManager.h"
#include "mnt_control_app/ControlGate.h"
#include "mnt_control_app/ControlAuthException.h"
#include "mnt_control_app/Transport.h"
#include "ControlAppAuthenticator.h"
#include "thread/ThreadCollector.h"
#include "log_writer/LogWriter.h"


/**
 * ControlClient exception sclass.
 *
 * Solves problem with catching errors that occured when processing
 * control client message (not IO errors).
 */
class ControlException : public Exception
{
public:
  ControlException(const Exception* parent) : Exception(parent->getMessage()) { }
  ControlException(const TCHAR* message) : Exception(message) { }
  virtual ~ControlException() { };
};


class ControlClient : public Thread
{
public:

  ControlClient(Transport* transport,
    RfbClientManager* rfbClientManager,
    ControlAppAuthenticator* authenticator,
    HANDLE pipeHandle,
    LogWriter* log);

  virtual ~ControlClient();

protected:
 
  virtual void execute();

  virtual void onTerminate();

private:

  void sendError(const TCHAR* message) throw(IOException);

  void authMsgRcdv() throw(IOException);

  void getClientsListMsgRcvd() throw(IOException);
  void getServerInfoMsgRcvd() throw(IOException);

  void reloadConfigMsgRcvd() throw(IOException);
 
  void disconnectAllMsgRcvd() throw(IOException);
 
  void shutdownMsgRcvd() throw(IOException);
 
  void addClientMsgRcvd() throw(IOException);

  void connectToTcpDispatcher() throw(IOException);

  void setServerConfigMsgRcvd() throw(IOException);

  void getServerConfigMsgRcvd() throw(IOException);
 
  void getShowTrayIconFlagMsgRcvd() throw(IOException);

  void updateTvnControlProcessIdMsgRcvd() throw(IOException);

  void sharePrimaryIdMsgRcvd();

  void shareDisplayIdMsgRcvd();

  void shareWindowIdMsgRcvd();
 
  void shareRectIdMsgRcvd();

  void shareFullIdMsgRcvd();

  void shareAppIdMsgRcvd();

private:

  Transport* m_transport;

  Channel* m_stream;


  ControlGate* m_gate;
  HANDLE m_pipeHandle;

  RfbClientManager* m_rfbClientManager;


  bool m_authPassed;
  bool m_repeatAuthPassed;
  UINT32 m_authReqMessageId;

  ControlAppAuthenticator* m_authenticator;

  // A connection identifier will be used by a viewer to connect to
  // the server across a tcp dispatcher.
  unsigned int m_tcpDispId;
  StringStorage m_gotDispatcherName;
  LocalMutex m_tcpDispValuesMutex;

  ThreadCollector m_outgoingConnectionThreadCollector;

  LogWriter* m_log;

  /**
   * Array of client messages that needs client to be auth.
   */
  static const UINT32 REQUIRES_AUTH[];
  static const UINT32 WITHOUT_AUTH[];
};

