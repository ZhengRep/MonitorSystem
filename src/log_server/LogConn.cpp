#include "LogConn.h"

#include "LogConnAuthListener.h"
#include "thread/AutoLock.h"
#include "SecurityPipeServer.h"
#include "io_lib/DataInputStream.h"
#include "io_lib/DataOutputStream.h"
#include "utils/DateTime.h"

LogConn::LogConn(Channel* channel, LogConnAuthListener* extAuthListener,
  LogListener* extLogListener, unsigned char logLevel)
  : m_serviceChannel(channel),
  m_extAuthListener(extAuthListener),
  m_extLogListener(extLogListener),
  m_logListenChannel(0),
  m_levelSendChannel(0),
  m_handle(0),
  m_logLevel(logLevel)
{
  // Main work must be in other thread to return control to a LogConn caller
  resume();
}

LogConn::~LogConn()
{
  terminate();
  m_logLevelSender.terminate();
  wait();
  m_logLevelSender.wait();

  if (m_serviceChannel != 0) delete m_serviceChannel;
  if (m_logListenChannel != 0) delete m_logListenChannel;
  if (m_levelSendChannel != 0) delete m_levelSendChannel;
}

void LogConn::onTerminate()
{
  {
    AutoLock al(&m_channelMutex);
    try {
      if (m_logListenChannel != 0) m_logListenChannel->close();
      if (m_levelSendChannel != 0) m_levelSendChannel->close();
    } catch (...) {
    }
  }
}

void LogConn::close()
{
  terminate();
}

void LogConn::changeLogLevel(unsigned char newLevel)
{
  {
    AutoLock al(&m_logLevelMutex);
    m_logLevel = newLevel; // This will become useless since m_logLevelSender
    // will be started.
    m_logLevelSender.updateLevel(m_logLevel);
  }
}

void LogConn::assignConnection()
{
  try {
    const unsigned int maxLogLineLength = 0x10000;
    const unsigned int maxChangeLevelMessageLength = 0x10;

    SecurityPipeServer secLogPipeServer(m_serviceChannel, maxLogLineLength);
    SecurityPipeServer secLevelPipeServer(m_serviceChannel, maxChangeLevelMessageLength);

    {
      AutoLock al(&m_channelMutex);
      m_logListenChannel = secLogPipeServer.getChannel();
      m_levelSendChannel = secLevelPipeServer.getChannel();
    }

    delete m_serviceChannel;
    m_serviceChannel = 0;
  } catch (...) {
    m_extAuthListener->onLogConnAuth(this, false, 0);
    throw;
  }
}

void LogConn::dispatch()
{
  DataInputStream input(m_logListenChannel);
  DataOutputStream output(m_logListenChannel);
  // Simple dispatcher (normal phase)
  StringStorage logMess;
  while (!isTerminating()) {
    UINT32 processId = input.readUInt32();
    UINT32 threadId = input.readUInt32();
    DateTime dt(input.readUInt64());
    UINT8 level = input.readUInt8();
    // Receive log message
    input.readUTF8(&logMess);

    m_extLogListener->onLog(m_handle, processId, threadId, &dt,
      level, logMess.getString());
  }
}

void LogConn::execute()
{
  try {
    assignConnection();
    // In success go to normal phase
    DataInputStream input(m_logListenChannel);

    StringStorage fileName;
    input.readUTF8(&fileName);
    m_handle = m_extAuthListener->onLogConnAuth(this,
      true,
      fileName.getString());

    m_logLevelSender.startSender(m_levelSendChannel);
    // Send first log level value
    {
      AutoLock al(&m_logLevelMutex);
      m_logLevelSender.updateLevel(m_logLevel);
    }

    dispatch();
  } catch (Exception& e) {
    StringStorage errMess;
    errMess.format(_T("The log connection has failed: %s"), e.getMessage());
    m_extLogListener->onAnErrorFromLogConn(errMess.getString());
  }
  m_extAuthListener->onDisconnect(this);
}
