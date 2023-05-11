#pragma once
#include "Utils/StringVector.h"
#include "Utils/Exception.h"
#include "PortMappingContainer.h"
#include "IpAccessControl.h"
#include "thread/AutoLock.h"
#include "thread/LocalMutex.h"
#include "IpAccessRule.h"
#include "io_lib/DataInputStream.h"
#include "io_lib/DataOutputStream.h"
#include "region/RectSerializer.h"

#include <ShlObj.h>

class ServerConfig: public Lockable
{
public:
  static const unsigned int MINIMAL_POLLING_INTERVAL = 30;
  static const unsigned int MINIMAL_LOCAL_INPUT_PRIORITY_TIMEOUT = 1;
  static const unsigned int MINIMAL_QUERY_TIMEOUT = 1;

  static const int MNT_PASSWORD_SIZE = 8;

  //Enum defines server action when last client disconnects from the vnc sever
  enum DisconnectAction {
    DA_DO_NOTHING = 0,
    DA_LOCK_WORKSTATION = 1,
    DA_LOGOUT_WORKSTATION = 2
  };

public:
  ServerConfig();
  virtual ~ServerConfig();

  void serialize(DataOutputStream* output) throw(Exception);
  void deserialize(DataInputStream* input) throw(Exception);
  virtual void lock() {
    m_objectCS.lock();
  }

  virtual void unlock() {
    m_objectCS.unlock();
  }

  bool isControlAuthEnabled();
  void useControlAuth(bool useAuth);

  bool getControlAuthAlwaysChecking();
  void setControlAuthAlwaysChecking(bool value);

  void setRfbPort(int port);
  int getRfbPort();

  void setHttpPort(int port);
  int getHttpPort();

  void enableFileTransfers(bool enabled);
  bool isFileTransfersEnabled();

  void enableRemovingDesktopWallpaper(bool enabled);
  bool isRemovingDesktopWallpaperEnabled();

  void setDisconnectAction(DisconnectAction action);
  DisconnectAction getDisconnectAction();

  bool getD3DIsAllowed();
  void setD3DAllowing(bool value);

  bool getMirrorIsAllowed();
  void setMirrorAllowing(bool value);

  bool isAcceptingRfbConnections();
  void acceptRfbConnections(bool accept);

  void getPrimaryPassword(unsigned char* password);
  void setPrimaryPassword(const unsigned char* value);

  void getReadOnlyPassword(unsigned char* password);
  void setReadOnlyPassword(const unsigned char* value);

  void getControlPassword(unsigned char* password);
  void setControlPassword(const unsigned char* password);

  bool hasPrimaryPassword();
  bool hasReadOnlyPassword();
  bool hasControlPassword();

  void deletePrimaryPassword();
  void deleteReadOnlyPassword();
  void deleteControlPassword();

  bool isUsingAuthentication();

  void useAuthentication(bool enabled);

  bool isOnlyLoopbackConnectionsAllowed();

  void acceptOnlyLoopbackConnections(bool enabled);

  bool isAcceptingHttpConnections();

  void acceptHttpConnections(bool accept);

  bool isAppletParamInUrlEnabled();

  void enableAppletParamInUrl(bool enabled);

  int getLogLevel();

  void setLogLevel(int logLevel);

  //
 // Sharing configuration
 //

  bool isAlwaysShared();

  bool isNeverShared();

  bool isDisconnectingExistingClients();

  void setAlwaysShared(bool enabled);

  void setNeverShared(bool enabled);

  void disconnectExistingClients(bool disconnectExisting);

  void setPollingInterval(unsigned int interval);

  unsigned int getPollingInterval();

  //
  // Input handling config
  //

  void blockRemoteInput(bool blockEnabled);

  bool isBlockingRemoteInput();

  void setLocalInputPriority(bool localPriority);

  bool isLocalInputPriorityEnabled();

  unsigned int getLocalInputPriorityTimeout();

  void setLocalInputPriorityTimeout(unsigned int value);

  void blockLocalInput(bool enabled);

  bool isBlockingLocalInput();

  //
  // Query config
  //

  unsigned int getQueryTimeout();

  void setQueryTimeout(unsigned int timeout);

  bool isDefaultActionAccept();
  void setDefaultActionToAccept(bool accept);

  PortMappingContainer* getPortMappingContainer();

  //
  // Ip access control config
  //

  // Remark: not-thread safe method, use lock / unlock methods of this class
  // to lock and unlock server configuration.
  IpAccessControl* getAccessControl();

  IpAccessRule::ActionType getActionByAddress(unsigned long ip);

  void allowLoopbackConnections(bool allow);

  bool isLoopbackConnectionsAllowed();

  //
  // Video regions
  //

  // Remark: not-thread safe method, use lock / unlock methods of this class
  // to lock and unlock server configuration.
  StringVector* getVideoClassNames();

  std::vector<Rect>* getVideoRects();

  //
  // Other
  //

  unsigned int getVideoRecognitionInterval();
  void setVideoRecognitionInterval(unsigned int interval);

  int  getIdleTimeout();
  void setIdleTimeout(int timeout);

  void saveLogToAllUsersPath(bool enabled);
  bool isSaveLogToAllUsersPathFlagEnabled();

  void setGrabTransparentWindowsFlag(bool grab);
  bool getGrabTransparentWindowsFlag();

  bool getShowTrayIconFlag();
  void setShowTrayIconFlag(bool val);

  bool getConnectToRdpFlag();
  void setConnectToRdpFlag(bool val);

  void getLogFileDir(StringStorage* logFileDir);
  void setLogFileDir(const TCHAR* logFileDir);

protected:
  //Server port numbers
  int m_rfbPort;
  int m_httpPort;

  bool m_enableFileTransfers;
  bool m_removeWallpaper;
  bool m_D3DAllowed;
  bool m_mirrorDriverAllowed;

  DisconnectAction m_disconnectAction;

  bool m_acceptRfbConnections;
  bool m_acceptHttpConnections;

  unsigned char m_primaryPassword[MNT_PASSWORD_SIZE];
  unsigned char m_readonlyPassword[MNT_PASSWORD_SIZE];
  unsigned char m_controlPassword[MNT_PASSWORD_SIZE];

  //Configurator from administration tab
  bool m_useAuthentication;
  bool m_onlyLoopbackConnections;
  bool m_enableAppletParamInUrl;
  int m_logLevel;
  bool m_useControlAuth;
  bool m_controlAuthAlwaysChecking;

  //Sharing configuration
  bool m_alwaysShared;
  bool m_neverShared;
  bool m_disconnectClients;

  //Polling configuration
  unsigned int m_pollingInterval;

  bool m_blockRemoteInput;
  bool m_blockLocalInput;

  bool m_localInputPriority;

  unsigned int m_localInputPriorityTimeout;
  bool m_defaultActionAccept;

  //query IpAccessControl record
  unsigned int m_queryTimeout;

  PortMappingContainer m_portMappings;

  //
  // Ip access control config
  //

  IpAccessControl m_accessControlContainer;
  bool m_allowLoopbackConnections;

  //
  // Video regions
  //

  // Defined by window class name
  StringVector m_videoClassNames;
  // Defined by rectangle coords in "dXxdY+X0+Y0" format, as in -sharerect command line option
  std::vector<Rect> m_videoRects;

  unsigned int m_videoRecognitionInterval;
  bool m_grabTransparentWindows;

  // Socket timeout to disconnect inactive clients, in seconds
  int m_idleTimeout;

  //Flag that deteminates whare log file directory will be
  bool m_saveLogToAllUsersPath;
  bool m_showTrayIcon;
  bool m_connectToRdp;

  StringStorage m_logFilePath;

 private:
   bool m_hasPrimaryPassword;
   bool m_hasReadOnlyPassword;
   bool m_hasControlPassword;

   LocalMutex m_objectCS;
};

