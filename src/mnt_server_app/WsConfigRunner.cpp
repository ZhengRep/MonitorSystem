#include "WsConfigRunner.h"
#include "win_system/CurrentConsoleProcess.h"
#include "win_system/Environment.h"
#include "server_config_lib/Configurator.h"
#include "mnt_control_app/ControlCommandLine.h"

WsConfigRunner::WsConfigRunner(Logger* logger, bool serviceMode)
  : m_serviceMode(serviceMode),
  m_log(logger)
{
  resume();
}

WsConfigRunner::~WsConfigRunner()
{
  terminate();
  wait();
}

void WsConfigRunner::execute()
{
  Process* process = 0;

  try {
    // Prepare path to executable.
    StringStorage pathToBin;
    Environment::getCurrentModulePath(&pathToBin);
    pathToBin.quoteSelf();
    // Prepare arguments.
    StringStorage args;
    args.format(_T("%s %s"),
      m_serviceMode ? ControlCommandLine::CONTROL_SERVICE :
      ControlCommandLine::CONTROL_APPLICATION,
      ControlCommandLine::SLAVE_MODE);
    // Start process.
    process = new Process(pathToBin.getString(), args.getString());
    process->start();
  } catch (Exception& e) {
    m_log.error(_T("Cannot start the WsControl process (%s)"), e.getMessage());
  }

  if (process != 0) {
    delete process;
  }
}
