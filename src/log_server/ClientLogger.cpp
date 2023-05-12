#include "ClientLogger.h"

#include "win_system/PipeClient.h"
#include "SecurityPipeClient.h"
#include "utils/DateTime.h"
#include "utils/Macros.h"

ClientLogger::ClientLogger(const TCHAR* publicPipeName, const TCHAR* logFileName)
  : LogDump(false, true),
  m_logSendingChan(0),
  m_levListenChan(0),
  m_logInput(0),
  m_logOutput(0),
  m_logBarrier(0),
  m_logFileName(logFileName),
  m_publicPipeName(publicPipeName)
{
}

ClientLogger::~ClientLogger()
{
  freeResources();
}

void ClientLogger::freeResources()
{
  if (m_levListenChan != 0) m_levListenChan->close();
  terminate();
  resume();
  wait();
  if (m_levListenChan != 0) delete m_levListenChan;

  if (m_logOutput != 0) delete m_logOutput;
  if (m_logInput != 0) delete m_logInput;
  if (m_logSendingChan != 0) delete m_logSendingChan;
}

void ClientLogger::connect()
{
  NamedPipe* svcChan = 0;
  try {
    // Try connect to log server
    svcChan = PipeClient::connect(m_publicPipeName.getString(), 0);
    SecurityPipeClient secLogPipeClient(svcChan, 0);
    m_logSendingChan = secLogPipeClient.getChannel();
    m_logInput = new DataInputStream(m_logSendingChan);
    m_logOutput = new DataOutputStream(m_logSendingChan);

    SecurityPipeClient secLevelPipeClient(svcChan, 0);
    m_levListenChan = secLevelPipeClient.getChannel();

    m_logOutput->writeUTF8(m_logFileName.getString());

    // Get log level by the m_levListenChan channel.
    DataInputStream m_levInput(m_levListenChan);

    unsigned char logLevel = m_levInput.readUInt8();
    setLogBarrier(logLevel);
  } catch (Exception& e) {
    if (svcChan != 0) delete svcChan;
    freeResources();
    StringStorage formattedException;
    formattedException.format(_T("Can't connect to the log server: %s"),
      e.getMessage());
    throw Exception(formattedException.getString());
  }
  if (svcChan != 0) delete svcChan;

  int logLevel = getLogBarrier();
  setLogBarrier(9);
  try {
    AutoLock al(&m_logWritingMut);
    writeLogDump();
  } catch (...) { // "finally"
    setLogBarrier(logLevel);
  }
  setLogBarrier(logLevel);
  terminateLogDumping();

  resume();
}

void ClientLogger::print(int logLevel, const TCHAR* line)
{
  UINT32 processId = GetCurrentProcessId();
  UINT32 threadId = GetCurrentThreadId();

  AutoLock al(&m_logWritingMut);
  updateLogDumpLines(processId, threadId, &unmove(DateTime::now()), logLevel, line);
  flush(processId, threadId, &unmove(DateTime::now()), logLevel, line);
}

bool ClientLogger::acceptsLevel(int logLevel)
{
  return logDumpEnabled() || m_logOutput != 0 && logLevel <= getLogBarrier();
}

void ClientLogger::flush(unsigned int processId,
  unsigned int threadId,
  const DateTime* dt,
  int level,
  const TCHAR* message)
{
  AutoLock al(&m_logWritingMut);

  if (level <= getLogBarrier()) {
    if (m_logOutput != 0) {
      try {
        m_logOutput->writeUInt32(processId);
        m_logOutput->writeUInt32(threadId);
        m_logOutput->writeUInt64(dt->getTime());
        m_logOutput->writeUInt8(level & 0xf);
        m_logOutput->writeUTF8(message);
      } catch (...) {
      }
    }
  }
}

int ClientLogger::getLogBarrier()
{
  AutoLock al(&m_logBarMut);
  return m_logBarrier;
}

void ClientLogger::setLogBarrier(int newLogBar)
{
  AutoLock al(&m_logBarMut);
  m_logBarrier = newLogBar & 0xf;
}

void ClientLogger::execute()
{
  DataInputStream levInput(m_levListenChan);
  try {
    while (!isTerminating()) {
      setLogBarrier(levInput.readUInt8());
    }
  } catch (...) {
  }
}

