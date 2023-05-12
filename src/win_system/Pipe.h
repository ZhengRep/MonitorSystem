#pragma once
#include "utils/CommonHeader.h"
#include "WindowsEvent.h"
#include "io_lib/Channel.h"
#include "thread/LocalMutex.h"

class Pipe
{
public:
  unsigned int getMaxPortionSize();

protected:
  Pipe(unsigned int maxPortionSize);
  virtual ~Pipe();

  size_t readByHandle(void* buffer, size_t len, HANDLE pipeHandle);
  size_t writeByHandle(void* buffer, size_t len, HANDLE pipeHandle);

  LocalMutex m_hPipeMutex;
  WindowsEvent m_readEvent;
  WindowsEvent m_writeEvent;

private:
  void checkPipeHandle(HANDLE pipeHandle);

  UINT64 m_totalWrote;
  UINT64 m_totalRead;
  unsigned int m_maxPortionSize;
};

