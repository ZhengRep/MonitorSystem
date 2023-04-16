#include "EmulatedAnonymousPipeFactory.h"
#include "SecurityAttributes.h"
#include "PipeServer.h"
#include "PipeClient.h"
#include <time.h>

EmulatedAnonymousPipeFactory::EmulatedAnonymousPipeFactory(unsigned int bufferSize, LogWriter* log)
  : m_bufferSize(bufferSize),
    m_log(log)
{
}

EmulatedAnonymousPipeFactory::~EmulatedAnonymousPipeFactory()
{
}

void EmulatedAnonymousPipeFactory::generatePipes(NamedPipe** serverPipe, bool serverInheritable, NamedPipe** clientPipe, bool clientInheritable)
{
  SecurityAttributes secAttr;
  secAttr.setInheritable();

  StringStorage randomName;
  getUniqPipeName(&randomName);
  PipeServer pipeServer(randomName.getString(), m_bufferSize, 0, 1000);
  *clientPipe = PipeClient::connect(randomName.getString(), m_bufferSize);
  *serverPipe = pipeServer.accept();

  HANDLE hThisSideWrite = (*serverPipe)->getHandle();
  HANDLE hOtherSideRead = (*clientPipe)->getHandle();

  const TCHAR* errMess = _T("Cannot disable inheritance for named pipe");
  if (!serverInheritable) {
    if (SetHandleInformation(hThisSideWrite, HANDLE_FLAG_INHERIT, 0) == 0) {
      SystemException(errMess);
    }
  }
  if (!clientInheritable) {
    if (SetHandleInformation(hOtherSideRead, HANDLE_FLAG_INHERIT, 0) == 0) {
      SystemException(errMess);
    }
  }
}

void EmulatedAnonymousPipeFactory::getUniqPipeName(StringStorage* result)
{
  srand((unsigned)time(0));
  for (int i = 0; i < 20; i++) {
    result->appendChar('a' + rand() % ('z' - 'a'));
  }
}
