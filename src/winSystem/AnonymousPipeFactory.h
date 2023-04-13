#pragma once
#include "AnonymousPipe.h"
#include "log_writer/LogWriter.h"

class AnonymousPipeFactory
{
public:
  AnonymousPipeFactory(unsigned int bufferSize, LogWriter* log);
  virtual ~AnonymousPipeFactory();

  void generatePipes(AnonymousPipe** firstSide, bool firstSideIsInheritable, AnonymousPipe** secondSide, bool secondSideIsInheritable);

private:
  LogWriter* m_log;
  unsigned int m_bufferSize;
};

