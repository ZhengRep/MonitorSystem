#pragma once
#include "io_lib/Channel.h"
#include "thread/LocalMutex.h"
#include "winSystem/WindowsEvent.h"
#include "log_writer/LogWriter.h"

class ReconnectingChannel: public Channel
{
public:
  ReconnectingChannel(unsigned int timeOut, LogWriter* log);
  virtual ~ReconnectingChannel();

  virtual size_t read(void* buffer, size_t len);
  virtual size_t write(const void* buffer, size_t len);

  void replaceChannel(Channel* newChannel);
  virtual void close();
  virtual size_t available() { return 0; }

private:
  Channel* getChannel(const TCHAR* funName);
  void waitForReconnect(const TCHAR* funName, Channel* channel);

  bool m_isClosed;
  Channel* m_channel;
  Channel* m_oldChannel;
  bool m_chanWasChanged;
  LocalMutex m_chanMut;
  WindowsEvent m_timer;
  unsigned int m_timeOut;
  LogWriter* m_log;
};

