#pragma once
#include "io_lib/Channel.h"
#include "io_lib/DataInputStream.h"
#include "io_lib/DataOutputStream.h"
#include "CapContainer.h"
#include "region/Dimension.h"
#include "rfb/PixelFormat.h"
#include "ClientAuthListener.h"

class RfbInitializer
{
public:
  RfbInitializer(Channel* stream, ClientAuthListener* extAuthListener,
    RfbClient* client, bool authAllowed);
  virtual ~RfbInitializer();

  void authPhase();
  void afterAuthPhase(const CapContainer* srvToClCaps, const CapContainer* clToSrvCaps,
    const CapContainer* encCaps, const Dimension* dim, const PixelFormat* pf);

  bool getSharedFlag() const { return m_shared; }
  bool getViewOnlyAuth() const { return m_viewOnlyAuth; }

  bool getTightEnabledFlag() const { return m_tightEnabled; }

protected:
  void initVersion();
  void checkForLoopback();
  void initAuthenticate();
  void readClientInit();
  void sendServerInit(const Dimension* dim, const PixelFormat* pf);
  void sendDesktopName();
  void sendInteractionCaps(const CapContainer* srvToClCaps,
    const CapContainer* clToSrvCaps, const CapContainer* encCaps);

  void doAuth(UINT32 authType);
  void doTightAuth();
  void doMntAuth();
  void doAuthNone();
  void checkForBan();

  // Parse RFB version string which should look like "RFB 003.008\n"
  unsigned int getProtocolMinorVersion(const char str[12]) throw(Exception);

  DataOutputStream* m_output;
  DataInputStream* m_input;

  bool m_shared;
  unsigned int m_minorVerNum;
  bool m_viewOnlyAuth;
  bool m_tightEnabled;
  bool m_authAllowed;

  ClientAuthListener* m_extAuthListener;
  RfbClient* m_client;
};

