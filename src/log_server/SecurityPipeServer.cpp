#include "SecurityPipeServer.h"

#include "winSystem/PipeServer.h"
#include "winSystem/PipeClient.h"
#include "io_lib/DataInputStream.h"
#include "io_lib/DataOutputStream.h"
#include "winSystem/Environment.h"
#include "winSystem/WinHandles.h"
#include "ConnectionTimer.h"
#include <time.h>

SecurityPipeServer::SecurityPipeServer(Channel* tempPublChan, unsigned int bufferSize)
  : m_secChannel(0),
  m_bufferSize(bufferSize)
{
  try {
    generateSecConnection(tempPublChan);
    makeSure();
  } catch (...) {
    if (m_secChannel != 0) delete m_secChannel;
    throw;
  }
}

SecurityPipeServer::~SecurityPipeServer()
{
}

Channel* SecurityPipeServer::getChannel()
{
  Channel* chan = m_secChannel;
  m_secChannel = 0;
  return chan;
}

void SecurityPipeServer::generateSecConnection(Channel* tempPublChan)
{
  NamedPipe* otherSideChannel = 0;

  try {
    DataInputStream svcInput(tempPublChan);
    DataOutputStream svcOutput(tempPublChan);

    UINT32 procId = svcInput.readUInt32();
    // Check the id. If it is "right" process then generate
    // transport handles specially for it.
    if (!Environment::isItTheSamePathAsCurrent(procId)) {
      throw Exception(_T("The process that has requested connection")
        _T(" to the log server has not access right"));
    }

    // Give to process exclusive pipe handles
    StringStorage randomName;
    srand((unsigned)time(0));
    for (int i = 0; i < 20; i++) {
      randomName.appendChar('a' + rand() % ('z' - 'a'));
    }
    PipeServer pipeServer(randomName.getString(), m_bufferSize, 0, 1000);
    otherSideChannel = PipeClient::connect(randomName.getString(), m_bufferSize);
    m_secChannel = pipeServer.accept();

    HANDLE otherSideHandle = otherSideChannel->getHandle();
    svcOutput.writeUInt64((UINT64)WinHandles::assignHandleFor(otherSideHandle, procId, false, false));
  } catch (...) {
    if (otherSideChannel != 0) delete otherSideChannel;
    throw;
  }
  if (otherSideChannel != 0) delete otherSideChannel;
}

void SecurityPipeServer::onTimeTimer()
{
  m_secChannel->close();
}

void SecurityPipeServer::makeSure()
{
  DataInputStream inputGate(m_secChannel);
  DataOutputStream outputGate(m_secChannel);
  // Acknowledgement of the receipt
  unsigned int timeOut = 10000; // milliseconds
  ConnectionTimer connTimer(this, timeOut);
  // If no byte received during time out interval connTimer has break
  // read operation and the make sure throws an Exception.
  inputGate.readInt8();
}
