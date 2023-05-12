#include "ControlPipeName.h"
#include "mnt_server_app/NamingDefs.h"
#include "win_system/WTS.h"

void ControlPipeName::createPipeName(bool forService, StringStorage* pipeName, LogWriter* log)
{
  if (forService) {
    pipeName->setString(
      ServerApplicationNames::FOR_SERVICE_CONTROL_APP_PIPE_NAME);
  } else {
    pipeName->format(_T("%s_On_Session_%d"),
      ServerApplicationNames::FOR_APP_CONTROL_APP_SERVICE_PIPE_NAME,
      WTS::getActiveConsoleSessionId(log));
  }
}
