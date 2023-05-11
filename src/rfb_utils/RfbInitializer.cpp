#include "RfbInitializer.h"
#include "vnc_server_app/NamingDefs.h"
#include "winSystem/Environment.h"
#include <time.h>
#include "Utils/AnsiStringStorage.h"
#include "server_config_lib/ServerConfig.h"
#include "server_config_lib/Configurator.h"
#include "rfb/AuthDefs.h"
#include "rfb/VendorDefs.h"
#include "Utils/MntPassCrypt.h"

RfbInitializer::RfbInitializer(Channel* stream, ClientAuthListener* extAuthListener, RfbClient* client, bool authAllowed)
  : m_shared(false),
  m_tightEnabled(false),
  m_minorVerNum(0),
  m_extAuthListener(extAuthListener),
  m_client(client),
  m_authAllowed(authAllowed),
  m_viewOnlyAuth(false)
{
  m_output = new DataOutputStream(stream);
  m_input = new DataInputStream(stream);
}

RfbInitializer::~RfbInitializer()
{
  delete m_output;
  delete m_input;
}

void RfbInitializer::authPhase()
{
  initVersion();
  initAuthenticate();
  readClientInit();
}

void RfbInitializer::afterAuthPhase(const CapContainer* srvToClCaps, const CapContainer* clToSrvCaps, const CapContainer* encCaps, const Dimension* dim, const PixelFormat* pf)
{
  sendServerInit(dim, pf);
  sendDesktopName();
  if (m_tightEnabled) {
    sendInteractionCaps(srvToClCaps, clToSrvCaps, encCaps);
  }
}

void RfbInitializer::initVersion()
{
  char initVersionMsg[] = "RFB 003.008\n";
  char clientVersionMsg[13];
  size_t msgLen = 12;
  m_output->writeFully(initVersionMsg, msgLen);
  m_input->readFully(clientVersionMsg, msgLen);
  clientVersionMsg[12] = 0;
  m_minorVerNum = getProtocolMinorVersion(clientVersionMsg);

  try {
    checkForLoopback();
    // Checking for a ban before auth and then after.
    checkForBan();
  } catch (Exception& e) {
    if (m_minorVerNum == 3) {
      m_output->writeUInt32(0);
    } else {
      m_output->writeUInt8(0);
    }
    AnsiStringStorage reason(&StringStorage(e.getMessage()));
    unsigned int reasonLen = (unsigned int)reason.getLength();
    _ASSERT(reasonLen == reason.getLength());

    m_output->writeUInt32(reasonLen);
    m_output->writeFully(reason.getString(), reasonLen);

    throw;
  }
}

void RfbInitializer::checkForLoopback()
{
  SocketAddressIPv4 sockAddr;
  m_client->getSocketAddr(&sockAddr);
  struct sockaddr_in addrIn = sockAddr.getSockAddr();

  bool isLoopback = (unsigned long)addrIn.sin_addr.S_un.S_addr == 16777343;

  ServerConfig* srvConf = Configurator::getInstance()->getServerConfig();
  if (isLoopback && !srvConf->isLoopbackConnectionsAllowed()) {
    throw Exception(_T("Sorry, loopback connections are not enabled"));
  }
  if (srvConf->isOnlyLoopbackConnectionsAllowed() && !isLoopback) {
    throw Exception(_T("Your connection has been rejected"));
  }
}

void RfbInitializer::initAuthenticate()
{
  try {
    // Determine effective security type from the configuration.
    UINT32 primSecType = SecurityDefs::MNT;
    if (!Configurator::getInstance()->getServerConfig()->isUsingAuthentication()
      || !m_authAllowed) {
      primSecType = SecurityDefs::NONE;
    }
    // Here the protocol varies between versions 3.3 and 3.7+.
    if (m_minorVerNum >= 7) {
      // Send a list with two security types -- VNC-compatible security type
      // and a special code allowing to enable mnt protocol extensions.
      m_output->writeUInt8(2);
      m_output->writeUInt8(primSecType);
      m_output->writeUInt8(SecurityDefs::TIGHT);
      // Read what the client has actually selected.
      UINT8 clientSecType = m_input->readUInt8();
      if (clientSecType == SecurityDefs::TIGHT) {
        m_tightEnabled = true;
        doTightAuth();
      } else {
        if (clientSecType != primSecType) {
          throw Exception(_T("Security types do not match"));
        }
        doAuth(AuthDefs::convertFromSecurityType(clientSecType));
      }
    } else {
      // Just tell the client we will use the configured security type.
      m_output->writeUInt32(primSecType);
      doAuth(AuthDefs::convertFromSecurityType(primSecType));
    }
  } catch (AuthException& e) {
    if (m_minorVerNum >= 8) {
      AnsiStringStorage reason(&StringStorage(e.getMessage()));
      unsigned int reasonLen = (unsigned int)reason.getLength();
      _ASSERT(reasonLen == reason.getLength());

      m_output->writeUInt32(1); // FIXME: Use a named constant instead of 1.
      m_output->writeUInt32(reasonLen);
      m_output->writeFully(reason.getString(), reasonLen);
    }
    throw;
  }
}

void RfbInitializer::readClientInit()
{
  m_shared = m_input->readUInt8() != 0;
}

void RfbInitializer::sendServerInit(const Dimension* dim, const PixelFormat* pf)
{
  m_output->writeUInt16((UINT16)dim->width);
  m_output->writeUInt16((UINT16)dim->height);
  // Pixel format
  m_output->writeUInt8((UINT8)pf->bitsPerPixel);
  m_output->writeUInt8((UINT8)pf->colorDepth);
  m_output->writeUInt8((UINT8)pf->bigEndian);
  m_output->writeUInt8(1);
  m_output->writeUInt16((UINT16)pf->redMax);
  m_output->writeUInt16((UINT16)pf->greenMax);
  m_output->writeUInt16((UINT16)pf->blueMax);
  m_output->writeUInt8((UINT8)pf->redShift);
  m_output->writeUInt8((UINT8)pf->greenShift);
  m_output->writeUInt8((UINT8)pf->blueShift);
  // Padding
  m_output->writeUInt8(0);
  m_output->writeUInt16(0);
}

void RfbInitializer::sendDesktopName()
{
  StringStorage deskName;
  if (!Environment::getComputerName(&deskName)) {
    deskName.setString(DefaultNames::DEFAULT_COMPUTER_NAME);
  }

  AnsiStringStorage ansiName(&deskName);
  unsigned int dnLen = (unsigned int)ansiName.getLength();
  _ASSERT(dnLen == ansiName.getLength());

  m_output->writeUInt32(dnLen);
  m_output->writeFully(ansiName.getString(), dnLen);
}

void RfbInitializer::sendInteractionCaps(const CapContainer* srvToClCaps, const CapContainer* clToSrvCaps, const CapContainer* encCaps)
{
  m_output->writeUInt16(srvToClCaps->getCapCount());
  m_output->writeUInt16(clToSrvCaps->getCapCount());
  m_output->writeUInt16(encCaps->getCapCount());
  m_output->writeUInt16(0); // Pad

  srvToClCaps->sendCaps(m_output);
  clToSrvCaps->sendCaps(m_output);
  encCaps->sendCaps(m_output);
}

void RfbInitializer::doAuth(UINT32 authType)
{
  if (authType == AuthDefs::MNT) {
    doMntAuth();
  } else if (authType == AuthDefs::NONE) {
    doAuthNone();
  } else {
    throw Exception(_T(""));
  }
  // Perform additional work via a listener.
  m_extAuthListener->onCheckAccessControl(m_client);
  // Send authentication result.
  if (m_minorVerNum >= 8 || authType != AuthDefs::NONE) {
    m_output->writeUInt32(0); 
  }
}

void RfbInitializer::doTightAuth()
{
  // Negotiate tunneling.
  m_output->writeUInt32(0);
  // Negotiate authentication.
  if (Configurator::getInstance()->getServerConfig()->isUsingAuthentication()
    && m_authAllowed) {
    CapContainer authInfo;
    authInfo.addCap(AuthDefs::MNT, VendorDefs::STANDARD, AuthDefs::SIG_MNT);
    m_output->writeUInt32(authInfo.getCapCount());
    authInfo.sendCaps(m_output);
    // Read the security type selected by the client.
    UINT32 clientAuthValue = m_input->readUInt32();
    if (!authInfo.includes(clientAuthValue)) {
      throw Exception(_T(""));
    }
    doAuth(clientAuthValue);
  } else {
    m_output->writeUInt32(0);
    doAuth(AuthDefs::NONE);
  }
}

void RfbInitializer::doMntAuth()
{
  UINT8 challenge[16];
  srand((unsigned)time(0));
  for (int i = 0; i < sizeof(challenge); i++) {
    challenge[i] = rand() & 0xff;
  }

  m_output->writeFully(challenge, sizeof(challenge));
  UINT8 response[16];
  m_input->readFully(response, sizeof(response));
  // Checking for a ban after auth.
  checkForBan();

  // Comparing the challenge with the response.
  ServerConfig* srvConf = Configurator::getInstance()->getServerConfig();
  bool hasPrim = srvConf->hasPrimaryPassword();
  bool hasRdly = srvConf->hasReadOnlyPassword();

  if (!hasPrim && !hasRdly) {
    throw AuthException(_T("Server is not configured properly"));
  }

  if (hasPrim) {
    UINT8 crypPrimPass[8];
    srvConf->getPrimaryPassword(crypPrimPass);
    MntPassCrypt passCrypt;
    passCrypt.updatePlain(crypPrimPass);
    if (passCrypt.challengeAndResponseIsValid(challenge, response)) {
      return;
    }
  }
  if (hasRdly) {
    UINT8 crypReadOnlyPass[8];
    srvConf->getReadOnlyPassword(crypReadOnlyPass);
    MntPassCrypt passCrypt;
    passCrypt.updatePlain(crypReadOnlyPass);
    if (passCrypt.challengeAndResponseIsValid(challenge, response)) {
      m_viewOnlyAuth = true;
      return;
    }
  }
  // At this time we are sure that the client was typed an incorectly password.
  m_extAuthListener->onAuthFailed(m_client);

  StringStorage clientAddressStorage;
  m_client->getPeerHost(&clientAddressStorage);
  StringStorage errMess;
  errMess.format(_T("Authentication failed from %s"), clientAddressStorage.getString());

  throw AuthException(errMess.getString());
}

void RfbInitializer::doAuthNone()
{
}

unsigned int RfbInitializer::getProtocolMinorVersion(const char str[12]) throw(Exception)
{
  if (str[0] != 'R' || str[1] != 'F' || str[2] != 'B' || str[3] != ' ' ||
    !isdigit(str[4]) || !isdigit(str[5]) || !isdigit(str[6]) ||
    str[7] != '.' ||
    !isdigit(str[8]) || !isdigit(str[9]) || !isdigit(str[10]) ||
    str[11] != '\n') {
    throw Exception(_T("Invalid format of the RFB version message"));
  }

  unsigned int majorVersion =
    (str[4] - '0') * 100 + (str[5] - '0') * 10 + (str[6] - '0');
  if (majorVersion != 3) {
    throw Exception(_T("Unsupported RFB protocol version requested"));
  }

  unsigned int minorVersion =
    (str[8] - '0') * 100 + (str[9] - '0') * 10 + (str[10] - '0');
  return minorVersion;
}

void RfbInitializer::checkForBan()
{
  if (m_extAuthListener->onCheckForBan(m_client)) {
    throw AuthException(_T("Your connection has been rejected"));
  }
}
