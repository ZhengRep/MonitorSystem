#pragma once
#include "Utils/CommonHeader.h"
#include "WindowsEvent.h"
#include "io_lib/Channel.h"
#include "thread/LocalMutex.h"
#include "Pipe.h"

class NamedPipe: public Channel, public Pipe
{
public:
  NamedPipe(HANDLE hPipe, unsigned int maxPortionSize, bool asServer);
  virtual ~NamedPipe();
  void close();
  virtual size_t read(void* buffer, size_t len) throw(IOException);
  virtual size_t write(const void* buffer, size_t len) throw(IOException);
  virtual size_t available() { return 0; };

  virtual HANDLE getHandle() const;

private:
  void checkPipeHandle();

  HANDLE m_hPipe;
  LocalMutex M_hPipeMutex;
  StringStorage m_pipeName;

  WindowsEvent m_readEvent;
  WindowsEvent m_writeEvent;
  bool m_asServer;
};

