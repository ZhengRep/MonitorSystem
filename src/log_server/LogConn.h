#pragma once
#include "io_lib/Channel.h"
#include "thread/Thread.h"
#include "winSystem/Pipe.h"
#include "LogListener.h"
#include "LogLevelSender.h"

class LogConnAuthListener;

class LogConn : public Thread
{
public:
  LogConn(Channel* channel, LogConnAuthListener* extAuthListener,
    LogListener* extLogListener, unsigned char logLevel);
  virtual ~LogConn();

  // This function closes the connection that brings to object
  // termination.
  virtual void close();

  void changeLogLevel(unsigned char newLevel);

private:
  virtual void execute();
  virtual void onTerminate();

  void assignConnection();
  void dispatch();

  LogConnAuthListener* m_extAuthListener;
  LogListener* m_extLogListener;

  Channel* m_serviceChannel;
  Channel* m_logListenChannel;
  Channel* m_levelSendChannel;
  FileAccountHandle m_handle;
  LocalMutex m_channelMutex;

  unsigned char m_logLevel;
  LocalMutex m_logLevelMutex;

  LogLevelSender m_logLevelSender;
};

