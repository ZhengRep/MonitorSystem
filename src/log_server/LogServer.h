#pragma once
#include "ListenLogServer.h"
#include "LogConn.h"
#include "log_writer/FileAccount.h"
#include "utils/Singleton.h"
#include "LogConnAuthListener.h"
#include "thread/ThreadCollector.h"
#include "LogListener.h"
#include <list>
#include <map>

typedef std::list<LogConn*> ConnList;
typedef std::list<LogConn*>::iterator ConnListIter;
typedef std::map<FileAccountHandle, FileAccount*> FileAccountList;
typedef std::map<FileAccountHandle, FileAccount*>::iterator FAccountListIter;

class LogServer : private ConnectionListener, private LogConnAuthListener,
  private LogListener, private Singleton<LogServer>
{
public:
  LogServer(const TCHAR* publicPipeName);
  virtual ~LogServer();

  // The start() function allows to server accept connections. The function
  // was separated from the constructor to allow initialize the configuration
  // before.
  // @throw Exception on an error.
  void start(const TCHAR* logDir,
    unsigned char logLevel, size_t headerLineCount);

  void changeLogProps(const TCHAR* newLogDir, unsigned char newLevel);

private:
  virtual void onNewConnection(Channel* channel);
  virtual FileAccountHandle  onLogConnAuth(LogConn* logConn, bool success,
    const TCHAR* fileName);
  virtual void onDisconnect(LogConn* logConn);
  virtual void onLog(FileAccountHandle handle,
    unsigned int processId,
    unsigned int threadId,
    const DateTime* dt,
    int level,
    const TCHAR* message);
  virtual void onAnErrorFromLogConn(const TCHAR* message);

  FileAccountHandle addConnection(const TCHAR* fileName);

  // Stores all printed lines as a log header and stops it accumulation.
  void storeHeader();

  StringStorage m_publicPipeName;

  StringStorage m_logDir;
  unsigned char m_logLevel;
  ConnList m_notAuthConnList;
  ConnList m_connList;
  FileAccountList m_fileAccountList;
  LocalMutex m_logPropsMutex;

  ListenLogServer* m_listenLogServer;

  ThreadCollector m_threadCollector;

  size_t m_headerLineCount;
  size_t m_totalLogLines;
};

