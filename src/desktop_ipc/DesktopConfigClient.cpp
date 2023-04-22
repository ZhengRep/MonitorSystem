#include "DesktopConfigClient.h"
#include "thread/AutoLock.h"
#include "ReconnectException.h"

DesktopConfigClient::DesktopConfigClient(BlockingGate* forwGate)
  :DesktopServerProto(forwGate)
{
}

DesktopConfigClient::~DesktopConfigClient()
{
}

void DesktopConfigClient::updateByNewSettings(BlockingGate* gate)
{
  AutoLock al(gate);
  gate->writeUInt8(CONFIG_RELOAD_REQ);
  sendConfigSettings(gate);
}

bool DesktopConfigClient::isRemoteInputAllowed()
{
  bool result = false;
  try {
    AutoLock al(m_forwGate);
    m_forwGate->writeUInt8(SOFT_INPUT_ENABLING_REQ);
    m_forwGate->writeUInt64(m_lastInputTime.getTime());
    result = m_forwGate->readUInt8() != 0;
    m_lastInputTime = DateTime(m_forwGate->readUInt64());
  } catch (ReconnectException&) {
  }
  return result;
}
