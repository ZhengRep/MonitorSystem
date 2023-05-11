#include "EchoExtensionRequestHandler.h"
#include "rfb/MsgDefs.h"
#include "rfb/VendorDefs.h"
#include "thread/AutoLock.h"

EchoExtensionRequestHandler::EchoExtensionRequestHandler(RfbCodeRegistrator* registrator, RfbOutputGate* output, LogWriter* log, bool enabled)
  : m_output(output), m_enabled(enabled),
  m_log(log)
{
  if (!isEchoExtensionEnabled()) {
    return;
  }

  registrator->addClToSrvCap(ClientMsgDefs::ECHO_REQUEST, VendorDefs::MNT, EchoExtensionDefs::ECHO_REQUEST_SIG);

  registrator->addSrvToClCap(ServerMsgDefs::ECHO_RESPONSE, VendorDefs::MNT, EchoExtensionDefs::ECHO_RESPONSE_SIG);

  registrator->regCode(ClientMsgDefs::ECHO_REQUEST, this);

  m_log->message(_T("Echo extension request handler created"));
}

EchoExtensionRequestHandler::~EchoExtensionRequestHandler()
{
  m_log->message(_T("Echo extension request handler deleted"));
}

void EchoExtensionRequestHandler::onRequest(UINT32 reqCode, RfbInputGate* backGate)
{
  m_input = backGate;

  try {
    if (reqCode == ClientMsgDefs::ECHO_REQUEST) {
      UINT32 number = m_input->readUInt32();
      m_log->debug(_T("got echo request with number %d"), number);
      {
        AutoLock l(m_output);

        m_output->writeUInt32(ServerMsgDefs::ECHO_RESPONSE);
        m_output->writeUInt32(number);
        m_output->flush();
      }
    }
  } catch (Exception& someEx) {
    m_log->error(_T("Echo extension request failed: \"%s\""), someEx.getMessage());
  } // try / catch.

  m_input = NULL;
}

bool EchoExtensionRequestHandler::isEchoExtensionEnabled()
{
  return m_enabled;
}
