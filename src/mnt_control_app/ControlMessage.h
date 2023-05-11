#pragma once
#include "io_lib/DataOutputStream.h"
#include "io_lib/ByteArrayOutputStream.h"
#include "Utils/CommonHeader.h"
#include "RemoteException.h"
#include "ControlGate.h"

class ControlMessage : public DataOutputStream
{
public:
  ControlMessage(UINT32 messageId, ControlGate* gate,
    const TCHAR* passwordFile = 0, bool getPassFromConfigEnabled = false,
    bool forService = false);
  virtual ~ControlMessage();
  virtual void send() throw(IOException, RemoteException);

private:
  void sendData() throw(IOException);
  void checkRetCode() throw(IOException, RemoteException);
  void authFromFile();
  void authFromRegistry();


private:

  ControlGate* m_gate;

  ByteArrayOutputStream* m_tunnel;

  UINT32 m_messageId;

  StringStorage m_passwordFile;
  bool m_getPassFromConfigEnabled;
  bool m_forService;
};

