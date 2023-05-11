#pragma once
#include "Utils/CommonHeader.h"
#include "io_lib/Channel.h"
#include "io_lib/DataInputStream.h"
#include "io_lib/DataOutputStream.h"
#include "thread/Thread.h"
#include "log_writer/Logger.h"
#include "thread/AutoLock.h"
#include "log_writer/LogDump.h"

class ClientLogger : public Logger, private Thread, public LogDump
{
public:
  ClientLogger(const TCHAR* publicPipeName, const TCHAR* logFileName);
  virtual ~ClientLogger();
  void connect();

  virtual void print(int logLevel, const TCHAR* line);

  virtual bool acceptsLevel(int logLevel);

private:
  // Writes a log message.
  virtual void flush(unsigned int processId, unsigned int threadId, const DateTime* dt,
    int level, const TCHAR* message);

  void freeResources();

  virtual void execute();

  int getLogBarrier();
  void setLogBarrier(int newLogBar);

  Channel* m_logSendingChan;
  DataInputStream* m_logInput;
  DataOutputStream* m_logOutput;
  LocalMutex m_logWritingMut;

  Channel* m_levListenChan;

  int m_logBarrier;
  LocalMutex m_logBarMut;

  StringStorage m_logFileName;
  StringStorage m_publicPipeName;
};

