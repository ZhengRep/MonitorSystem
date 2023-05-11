#include "SecurityPipeClient.h"

#include "io_lib/DataInputStream.h"
#include "io_lib/DataOutputStream.h"
#include "winSystem/NamedPipe.h"

SecurityPipeClient::SecurityPipeClient(Channel* tempPublChan, unsigned int maxPortionSize)
  : m_secChannel(0)
{
  try {
    DataInputStream svcInput(tempPublChan);
    DataOutputStream svcOutput(tempPublChan);
    // Send our process id
    svcOutput.writeUInt32(GetCurrentProcessId());
    // Get working channel.
    HANDLE pipeHandle = (HANDLE)svcInput.readUInt64();
    m_secChannel = new NamedPipe(pipeHandle, maxPortionSize, false);
    DataOutputStream output(m_secChannel);
    // Send sure message.
    output.writeUInt8(255);
  } catch (...) {
    if (m_secChannel != 0) delete m_secChannel;
    throw;
  }
}

SecurityPipeClient::~SecurityPipeClient()
{
}

Channel* SecurityPipeClient::getChannel()
{
  Channel* chan = m_secChannel;
  m_secChannel = 0;
  return chan;
}
