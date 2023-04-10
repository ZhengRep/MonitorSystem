#pragma once
#include "RfbDispatcherListener.h"
#include "RfbCodeRegistrator.h"
#include "desktop/Desktop.h"
#include "network/RfbOutputGate.h"
#include "log_writer/LogWriter.h"

class ClipboardExchange:public RfbDispatcherListener, public Thread
{
public:
  ClipboardExchange(RfbCodeRegistrator* codeRegtor, Desktop* desktop, RfbOutputGate* output, bool viewOnly, LogWriter* log);
  virtual ~ClipboardExchange();
  void sendClipboard(const StringStorage* newClipboard);

protected:
  virtual void onRequest(UINT32 reqCode, RfbInputGate* input);
  virtual void execute();
  virtual void onTerminate();

private:
  void onRequestWorker(bool utf8flag, RfbInputGate* input);

  bool m_viewOnly;
  bool m_isUtf8ClipboardEnabled;
  Desktop* m_desktop;
  RfbOutputGate* m_output;

  WindowsEvent m_newClipWaiter;

  StringStorage m_storedClip;
  bool m_hasNewClip;
  LocalMutex m_storedClipMut;

  LogWriter* m_log;
};

