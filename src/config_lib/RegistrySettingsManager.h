#pragma once
#include "utils/StringStorage.h"
#include "win_system/RegistryKey.h"
#include "SettingsManager.h"

class RegistrySettingsManager: public SettingsManager
{
public:
  RegistrySettingsManager();
  RegistrySettingsManager(HKEY rootKey, const TCHAR* entry, SECURITY_ATTRIBUTES* sa = 0);
  virtual ~RegistrySettingsManager();

public:
  void setRegistryKey(HKEY rootKey, const TCHAR* entry, SECURITY_ATTRIBUTES* sa = 0);
  virtual bool isOk();

  virtual bool keyExist(const TCHAR* name);
  virtual bool deleteKey(const TCHAR* name);

  virtual bool getString(const TCHAR* name, StringStorage* value);
  virtual bool setString(const TCHAR* name, const TCHAR* value);

  virtual bool getLong(const TCHAR* name, long* value);
  virtual bool setLong(const TCHAR* name, long value);

  virtual bool getBoolean(const TCHAR* name, bool* value);
  virtual bool setBoolean(const TCHAR* name, bool value);

  virtual bool getUINT(const TCHAR* name, UINT* value);
  virtual bool setUINT(const TCHAR* name, UINT value);

  virtual bool getInt(const TCHAR* name, int* value);
  virtual bool setInt(const TCHAR* name, int value);

  virtual bool getByte(const TCHAR* name, char* value);
  virtual bool setByte(const TCHAR* name, char value);

  virtual bool getBinaryData(const TCHAR* name, void* value, size_t* size);
  virtual bool setBinaryData(const TCHAR* name, const void* value, size_t size);

protected:
  RegistryKey* m_key;

  void extractKeyName(const TCHAR* key, StringStorage* folder);
  void extractValueName(const TCHAR* key, StringStorage* keyName);
};

