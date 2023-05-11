#include "LogLevelSender.h"

#include "thread/AutoLock.h"

LogLevelSender::LogLevelSender()
  : m_outStream(0),
  m_logLevel(0),
  m_updateAvailable(false)
{
  resume();
}

LogLevelSender::~LogLevelSender()
{
  terminate();
  wait();
}

void LogLevelSender::startSender(OutputStream* outStream)
{
  AutoLock al(&m_updateMutex);
  m_outStream = outStream;
}

void LogLevelSender::onTerminate()
{
  m_sleeper.notify();
}

void LogLevelSender::updateLevel(unsigned char newLevel)
{
  AutoLock al(&m_updateMutex);
  m_updateAvailable = true;
  m_logLevel = newLevel;
  if (m_logLevel > 10) {
    m_logLevel = 10;
  }
  m_sleeper.notify();
}

void LogLevelSender::execute()
{
  try {
    while (!isTerminating()) {
      m_sleeper.waitForEvent();
      OutputStream* outStream;
      bool updateAvailable;
      unsigned char logLevel;
      {
        AutoLock al(&m_updateMutex);
        outStream = m_outStream;
        updateAvailable = m_updateAvailable;
        m_updateAvailable = false;
        logLevel = m_logLevel;
      }
      if (outStream != 0 && updateAvailable) {
        DataOutputStream output(outStream);
        output.writeUInt8(logLevel);
      }
    }
  } catch (...) { 
  }
}
