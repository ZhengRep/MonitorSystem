#pragma once
#include "Utils/CommonHeader.h"
#include "Utils/StringStorage.h"

class SettingsManager
{
public:
  SettingsManager(){};
  virtual ~SettingsManager(){};
public:
  virtual bool isOk() = 0;

  virtual bool keyExist(const TCHAR* name) = 0;
  virtual bool deleteKey(const TCHAR* name) = 0;

  virtual bool getString(const TCHAR* name, StringStorage* storage) = 0;
  virtual bool setString(const TCHAR* name, const TCHAR* value) = 0;

  virtual bool getLong(const TCHAR* name, long* value) = 0;
  virtual bool setLong(const TCHAR* name, long value) = 0;

  virtual bool getBoolean(const TCHAR* name, bool* value) = 0;
  virtual bool setBoolean(const TCHAR* name, bool value) = 0;

  virtual bool getUINT(const TCHAR* name, UINT* value) = 0;
  virtual bool setUINT(const TCHAR* name, UINT value) = 0;

  virtual bool getInt(const TCHAR* name, int* value) = 0;
  virtual bool setInt(const TCHAR* name, int value) = 0;

  virtual bool getByte(const TCHAR* name, char* value) = 0;
  virtual bool setByte(const TCHAR* name, char value) = 0;

  virtual bool getBinaryData(const TCHAR* name, void* value, size_t* size) = 0;
  virtual bool setBinaryData(const TCHAR* name, const void* value, size_t size) = 0;
};

