#pragma once
#include "Transport.h"
#include "winSystem/Pipe.h"
#include "winSystem/PipeServer.h"

class NamedPipeTransport: public Transport
{
public:
  NamedPipeTransport(NamedPipe* client);
  NamedPipeTransport(PipeServer* server);
  virtual ~NamedPipeTransport();
  virtual Channel* getIOStream();
  virtual Transport* accept() throw(Exception);
  virtual void close() throw(Exception);

private:
  NamedPipe* m_pipe;
  PipeServer* m_pipeServer;
};

