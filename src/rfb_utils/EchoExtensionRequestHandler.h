#pragma once
#include "Utils/inttypes.h"
#include "network/RfbInputGate.h"
#include "network/RfbOutputGate.h"
#include "rfb_utils/RfbCodeRegistrator.h"
#include "rfb_utils/RfbDispatcherListener.h"
#include "log_writer/LogWriter.h"

class EchoExtensionRequestHandler: public RfbDispatcherListener
{
public:
  EchoExtensionRequestHandler(RfbCodeRegistrator* registrator, RfbOutputGate* output,
    LogWriter* log, bool enabled = true);
  virtual ~EchoExtensionRequestHandler();

  virtual void onRequest(UINT32 reqCode, RfbInputGate* backGate);

  bool isEchoExtensionEnabled();

protected:
  RfbInputGate* m_input;
  RfbOutputGate* m_output;

  bool m_enabled;
  LogWriter* m_log;
};

