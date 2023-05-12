#include "ConfigServer.h"
#include "server_config_lib/Configurator.h"

ConfigServer::ConfigServer(DesktopSrvDispatcher* dispatcher, LogWriter* log)
  : DesktopServerProto(0),
  m_deskConf(log)
{
  dispatcher->registerNewHandle(CONFIG_RELOAD_REQ, this);
  dispatcher->registerNewHandle(SOFT_INPUT_ENABLING_REQ, this);
}

ConfigServer::~ConfigServer()
{
}

void ConfigServer::onRequest(UINT8 reqCode, BlockingGate* backGate)
{
  switch (reqCode) {
  case CONFIG_RELOAD_REQ:
    reloadSettings(backGate);
    break;
  case SOFT_INPUT_ENABLING_REQ:
    answerOnSoftInputEnablingReq(backGate);
    break;
  default:
    StringStorage errMess;
    errMess.format(_T("Unknown %d protocol code received from a pipe client"),
      (int)reqCode);
    throw Exception(errMess.getString());
    break;
  }
}

void ConfigServer::reloadSettings(BlockingGate* backGate)
{
  readConfigSettings(backGate);
  Configurator::getInstance()->notifyReload();
  m_deskConf.updateByNewSettings();
}

void ConfigServer::answerOnSoftInputEnablingReq(BlockingGate* backGate)
{
  m_deskConf.correctLastTime(DateTime(backGate->readUInt64()));

  bool allowed = m_deskConf.isRemoteInputAllowed();
  backGate->writeUInt8(allowed);
  backGate->writeUInt64(m_deskConf.getLastInputTime().getTime());
}
