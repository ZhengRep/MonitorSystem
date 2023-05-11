#pragma once
#include "ControlGate.h"
#include "RfbClientInfo.h"
#include "MntServerInfo.h"
#include "server_config_lib/ServerConfig.h"
#include "ControlMessage.h"
#include "RemoteException.h"
#include <list>

using namespace std;

class ControlProxy
{
public:
  ControlProxy(ControlGate* gate);
  virtual ~ControlProxy();
  void setPasswordProperties(const TCHAR* passwordFile,
    bool getPassFromConfigEnabled, bool forService);

  MntServerInfo getServerInfo() throw(IOException, RemoteException);
  void getClientsList(list<RfbClientInfo*>* clients) throw(IOException, RemoteException);
  void reloadServerConfig() throw(IOException, RemoteException);
  void disconnectAllClients() throw(IOException, RemoteException);
  void shutdownMnt() throw(IOException, RemoteException);
  void makeOutgoingConnection(const TCHAR* connectString, bool viewOnly) throw(IOException, RemoteException);
  void makeTcpDispatcherConnection(const TCHAR* connectString,
    const TCHAR* dispatcherName, const TCHAR* keyword, UINT32 connectionId);
  void sharePrimary();
  void shareDisplay(unsigned char displayNumber);
  void shareWindow(const StringStorage* shareWindowName);
  void shareRect(const Rect* shareRect);
  void shareFull();
  void shareApp(unsigned int procId);
  void setServerConfig(ServerConfig* config) throw(IOException, RemoteException);
  void getServerConfig(ServerConfig* config) throw(IOException, RemoteException);
  bool getShowTrayIconFlag() throw(IOException, RemoteException);
  void updateMntControlProcessId(DWORD processId) throw(IOException, RemoteException);

protected:
  ControlMessage* createMessage(DWORD messageId);

protected:
  /**
   * Transport for sending and recieving control proto messages.
   */
  ControlGate* m_gate;

  ControlMessage* m_message;
private:

  void releaseMessage();

  StringStorage m_passwordFile;
  bool m_getPassFromConfigEnabled;
  bool m_forService;

};

