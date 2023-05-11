#include "ControlCommand.h"
#include "RemoteException.h"
#include "ControlAuthException.h"
#include "Utils/CommonHeader.h"
#include "Utils/Exception.h"
#include "io_lib/IOException.h"

ControlCommand::ControlCommand(Command* command, Notificator* notificator)
  : m_command(command), m_notificator(notificator), m_successfull(false)
{
}

ControlCommand::~ControlCommand()
{
}

void ControlCommand::execute()
{
  _ASSERT(m_command != 0);

  m_successfull = true;

  if (m_command != NULL) {
    try {
      try {
        m_command->execute();
      } catch (IOException&) {
        if (m_notificator != 0) {
          m_notificator->notifyConnectionLost();
        }
        throw;
      } catch (ControlAuthException& authEx) {
        if ((m_notificator != 0) && !authEx.isSilent()) {
          m_notificator->notifyServerSideException(authEx.getMessage());
        }
        throw;
      } catch (RemoteException& someEx) {
        if (m_notificator != 0) {
          m_notificator->notifyServerSideException(someEx.getMessage());
        }
        throw;
      } catch (Exception&) {
        _ASSERT(FALSE);
        throw;
      }
    } catch (...) {
      m_successfull = false;
    }
  }
}

bool ControlCommand::executionResultOk() const
{
  return m_successfull;
}

