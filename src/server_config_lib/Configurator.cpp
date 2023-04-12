#include "Configurator.h"
#include "winSystem/Environment.h"
#include "wsconfig_lib/VncLogFilename.h"


Configurator::Configurator(bool isConfiguringService)
  : m_isConfiguringService(isConfiguringService), m_isConfigLoadedPartly(false),
  m_isFirstLoad(true), m_regSA(0)
{
  AutoLock al(&m_instanceMutex);
  if (s_instance != 0) {
    throw Exception(_T("Configurator instance already exists"));
  }
  s_instance = this;
  try {
    m_regSA = new RegistrySecurityAttributes();
  } catch (...) {
    // TODO: Place exception handler here.
  }
}

Configurator::~Configurator()
{
  if (m_regSA != 0) delete m_regSA;
}

bool Configurator::load()
{
  return load(m_isConfiguringService);
}

bool Configurator::save()
{
  return save(m_isConfiguringService);
}

Configurator* Configurator::getInstance()
{
  AutoLock al(&m_instanceMutex);
  _ASSERT(s_instance != NULL);
  return s_instance;
}

void Configurator::setInstance(Configurator* conf)
{
  s_instance = conf;
}

void Configurator::notifyReload()
{
  AutoLock l(&m_listeners);
  for (size_t i = 0; i < m_listeners.size(); i++) {
    m_listeners.at(i)->onConfigReload(getServerConfig());
  }
}

bool Configurator::savePortMappingContainer(SettingsManager* sm)
{
    return false;
}

bool Configurator::loadPortMappingContainer(SettingsManager* sm, PortMappingContainer* portMapping)
{
    return false;
}

bool Configurator::saveIpAccessControlContainer(SettingsManager* sm)
{
    return false;
}

bool Configurator::loadIpAccessControlContainer(SettingsManager* sm, IpAccessControl* ipContainer)
{
    return false;
}

bool Configurator::saveServerConfig(SettingsManager* sm)
{
    return false;
}

bool Configurator::loadServerConfig(SettingsManager* sm, ServerConfig* config)
{
    return false;
}

void Configurator::updateLogDirPath()
{
}

bool Configurator::saveQueryConfig(SettingsManager* sm)
{
    return false;
}

bool Configurator::loadQueryConfig(SettingsManager* sm, ServerConfig* config)
{
    return false;
}

bool Configurator::saveInputHandlingConfig(SettingsManager* sm)
{
    return false;
}

bool Configurator::loadInputHandlingConfig(SettingsManager* sm, ServerConfig* config)
{
    return false;
}

bool Configurator::saveVideoRegionConfig(SettingsManager* sm)
{
    return false;
}

bool Configurator::loadVideoRegionConfig(SettingsManager* sm, ServerConfig* config)
{
    return false;
}

bool Configurator::save(SettingsManager* sm)
{
    return false;
}

bool Configurator::load(SettingsManager* sm)
{
    return false;
}

bool Configurator::load(bool forService)
{
  bool isOk = false;

  HKEY rootKey = forService ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

  SECURITY_ATTRIBUTES* sa = 0;
  if (forService && m_regSA != 0) {
    sa = m_regSA->getServiceSA();
  }
  RegistrySettingM
}

bool Configurator::save(bool forService)
{
  return false;
}
