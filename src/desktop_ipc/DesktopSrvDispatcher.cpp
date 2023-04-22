#include "DesktopSrvDispatcher.h"
#include "ReconnectException.h"

DesktopSrvDispatcher::DesktopSrvDispatcher(BlockingGate* gate, AnEventListener* extErrorListener, LogWriter* log)
  : m_gate(gate),
  m_extErrorListener(extErrorListener),
  m_log(log)
{
}

DesktopSrvDispatcher::~DesktopSrvDispatcher()
{
  terminate();
  resume();
  wait();
}

void DesktopSrvDispatcher::registerNewHandle(UINT8 code, ClientListener* listener)
{
  m_handlers[code] = listener;
}

void DesktopSrvDispatcher::execute()
{
  while (!isTerminating()) {
    try {
      m_log->debug(_T("DesktopSrvDispatcher reading code"));
      UINT8 code = m_gate->readUInt8();
      m_log->debug(_T("DesktopSrvDispatcher, code %d recieved"), code);
      std::map<UINT8, ClientListener*>::iterator iter = m_handlers.find(code);
      if (iter == m_handlers.end()) {
        StringStorage errMess;
        errMess.format(_T("Unhandled %d code has been received from a client"), (int)code);
        throw Exception(errMess.getString());
      }
      (*iter).second->onRequest(code, m_gate);
    } catch (ReconnectException&) {
      m_log->message(_T("The DesktopServerApplication dispatcher has been reconnected"));
    } catch (Exception& e) {
      m_log->error(_T("The DesktopServerApplication dispatcher has failed with error: %s"), e.getMessage());
      notifyOnError();
      terminate();
    }
    Thread::yield();
  }
  m_log->message(_T("The DesktopServerApplication dispatcher has been stopped"));
}

void DesktopSrvDispatcher::onTerminate()
{
}

void DesktopSrvDispatcher::notifyOnError()
{
  if (m_extErrorListener) {
    m_extErrorListener->onAnObjectEvent();
  }
}
