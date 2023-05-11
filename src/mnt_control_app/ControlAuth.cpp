#include "ControlAuth.h"

#include "Utils/AnsiStringStorage.h"
#include "ControlProto.h"
#include "thread/AutoLock.h"
#include "Utils/DesCrypt.h"

ControlAuth::ControlAuth(ControlGate* gate, const TCHAR* password)
  : m_gate(gate)
{
  // Prepare data for authentication.
  StringStorage truncatedPass(password);
  truncatedPass.getSubstring(&truncatedPass, 0, ServerConfig::MNT_PASSWORD_SIZE - 1);

  AnsiStringStorage passwordAnsi(&truncatedPass);

  memset(m_password, 0, sizeof(m_password));
  memcpy(m_password, passwordAnsi.getString(), min(passwordAnsi.getLength(), sizeof(m_password)));


  m_gate->writeUInt32(ControlProto::AUTH_MSG_ID);
  m_gate->writeUInt32(0);

  authRfb();

  UINT8 result = m_gate->readUInt32();

  switch (result) {
  case ControlProto::REPLY_ERROR:
  {
    StringStorage authFailReason;

    m_gate->readUTF8(&authFailReason);

    throw ControlAuthException(authFailReason.getString());
  }
  break;
  case ControlProto::REPLY_OK:
    break;
  default:
    throw Exception(_T("Server return unknown auth result"));
    break;
  }
}

ControlAuth::~ControlAuth()
{
}

void ControlAuth::authRfb()
{
  UINT8 challenge[16];
  UINT8 response[16];

  m_gate->readFully(challenge, sizeof(challenge));

  DesCrypt desCrypt;

  desCrypt.encrypt(response, challenge, sizeof(challenge), m_password);

  m_gate->writeFully(response, sizeof(response));
}
