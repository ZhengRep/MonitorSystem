#pragma once
#include "utils/CommonHeader.h"
#include "WindowsEvent.h"
#include "io_lib/Channel.h"
#include "thread/LocalMutex.h"
#include "Pipe.h"
#include "log_writer/LogWriter.h"

class AnonymousPipe: public Pipe, public Channel
{
public:
  AnonymousPipe(HANDLE hWrite, HANDLE hRead, unsigned int maxPortionSize, LogWriter* log);
  virtual ~AnonymousPipe();
  void close();
  virtual size_t read(void* buffer, size_t len) throw(IOException);
  virtual size_t write(const void* buffer, size_t len) throw(IOException);

  virtual size_t available() { return 0; };
  HANDLE getWriteHandle() const;
  HANDLE getReadHandle() const;
  void assignHandlesFor(HANDLE hTargetProc, bool neededToClose, bool kekpCloseRight = false);
  void setTimeOut(unsigned int timeOut);

private:
  void checkPipeHandle(HANDLE handle);

  HANDLE m_hWrite;
  HANDLE m_hRead;
  bool m_neededToClose;
  unsigned int m_timeOut;
  
  LocalMutex m_hPipeMutex;
  WindowsEvent m_readEvent;
  WindowsEvent m_writeEvent;

  LogWriter* m_log;
};

