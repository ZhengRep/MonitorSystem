#pragma once
#include "thread/Thread.h"
#include "winSystem/WindowsEvent.h"
#include "io_lib/DataOutputStream.h"

class LogLevelSender : public Thread
{
public:
  LogLevelSender();
  virtual ~LogLevelSender();


  void startSender(OutputStream* outStream);
  void updateLevel(unsigned char newLevel);

private:
  virtual void execute();
  virtual void onTerminate();

  WindowsEvent m_sleeper;

  OutputStream* m_outStream;
  bool m_updateAvailable;
  unsigned char m_logLevel;
  LocalMutex m_updateMutex;
};

