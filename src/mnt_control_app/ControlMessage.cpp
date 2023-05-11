#include "ControlMessage.h"
#include "ControlAuth.h"
#include "ControlAuthDialog.h"
#include "ControlProto.h"
#include "server_config_lib/Configurator.h"
#include "config_lib/RegistrySettingsManager.h"
#include "Utils/MntPassCrypt.h"
#include "Utils/AnsiStringStorage.h"
#include "vnc_server_app/NamingDefs.h"
#include "file_lib/WinFile.h"
#include "mnt_server/resource.h"
#include <crtdbg.h>

ControlMessage::ControlMessage(UINT32 messageId, ControlGate* gate,
  const TCHAR* passwordFile, bool getPassFromConfigEnabled,
  bool forService)
  : DataOutputStream(0), m_messageId(messageId), m_gate(gate),
  m_passwordFile(passwordFile),
  m_getPassFromConfigEnabled(getPassFromConfigEnabled),
  m_forService(forService)
{
  m_tunnel = new ByteArrayOutputStream(2048);

  m_outputStream = m_tunnel;
}

ControlMessage::~ControlMessage()
{
  delete m_tunnel;
}

void ControlMessage::send()
{
  sendData();

  checkRetCode();
}

void ControlMessage::sendData()
{
  m_gate->writeUInt32(m_messageId);
  _ASSERT((UINT32)m_tunnel->size() == m_tunnel->size());
  m_gate->writeUInt32((UINT32)m_tunnel->size());
  m_gate->writeFully(m_tunnel->toByteArray(), m_tunnel->size());
}

void ControlMessage::checkRetCode()
{
  UINT32 messageId = m_gate->readUInt32();

  switch (messageId) {
  case ControlProto::REPLY_ERROR:
  {
    StringStorage message;
    m_gate->readUTF8(&message);
    throw RemoteException(message.getString());
  }
  break;
  case ControlProto::REPLY_AUTH_NEEDED:
    if (m_passwordFile.getLength() != 0) {
      authFromFile();
    } else if (m_getPassFromConfigEnabled) {
      authFromRegistry();
    } else {
      ControlAuthDialog authDialog;

      int retCode = authDialog.showModal();
      switch (retCode) {
      case IDCANCEL:
        throw ControlAuthException(StringTable::getString(IDS_USER_CANCEL_CONTROL_AUTH), true);
      case IDOK:
        ControlAuth auth(m_gate, authDialog.getPassword());
        send();
        break;
      }
    }
    break;
  case ControlProto::REPLY_OK:
    break;
  default:
    _ASSERT(FALSE);
    throw RemoteException(_T("Unknown ret code."));
  }
}

void ControlMessage::authFromFile()
{
  WinFile file(m_passwordFile.getString(), F_READ, FM_OPEN);
  char ansiBuff[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  file.read(ansiBuff, 8);
  for (int i = 0; i < 8; i++) {
    if (ansiBuff[i] == '\r' || ansiBuff[i] == '\n') {
      ansiBuff[i] = '\0';
    }
  }
  AnsiStringStorage ansiPwd(ansiBuff);
  StringStorage password;
  ansiPwd.toStringStorage(&password);
  ControlAuth auth(m_gate, password.getString());
  send();
}

void ControlMessage::authFromRegistry()
{
  HKEY rootKey = m_forService ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
  RegistrySettingsManager sm(rootKey, RegistryPaths::SERVER_PATH, 0);

  unsigned char hidePassword[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  unsigned char plainPassword[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  size_t passSize = sizeof(hidePassword);

  if (sm.getBinaryData(_T("ControlPassword"),
    hidePassword,
    &passSize)) {
    MntPassCrypt::getPlainPass(plainPassword, hidePassword);

    AnsiStringStorage plainAnsiString((char*)plainPassword);
    StringStorage password;
    plainAnsiString.toStringStorage(&password);
    // Clear ansi plain password from memory.
    memset(plainPassword, 0, sizeof(plainPassword));
    ControlAuth auth(m_gate, password.getString());

    send();
  } else {
    // Ignore errors for silent.
  }
}

