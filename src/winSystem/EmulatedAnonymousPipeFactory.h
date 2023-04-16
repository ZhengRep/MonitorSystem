#pragma once
#include "NamedPipe.h"
#include "log_writer/LogWriter.h"

class EmulatedAnonymousPipeFactory
{
public:
  EmulatedAnonymousPipeFactory(unsigned int bufferSize, LogWriter* log);
  virtual ~EmulatedAnonymousPipeFactory();

  void generatePipes(NamedPipe** serverPipe, bool serverInheritable,NamedPipe** clientPipe, bool clientInheritable);

private:
  void getUniqPipeName(StringStorage* result);

  LogWriter* m_log;
  unsigned int m_bufferSize;
};

