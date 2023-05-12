#pragma once
#include "utils/CommonHeader.h"

class RegistryKey
{
public:
  RegistryKey(HKEY rootKey, const TCHAR* entry, bool createIfNotExists = true, SECURITY_ATTRIBUTES* sa = 0);
  RegistryKey(RegistryKey* rootKey, const TCHAR* entry, bool createIfNotExists = true, SECURITY_ATTRIBUTES* sa = 0);
  RegistryKey(HKEY rootKey);
  // Default contructor for a defer initialization.
  RegistryKey();

public:
  virtual ~RegistryKey();
  void open(HKEY rootKey, const TCHAR* entry, bool createIfNotExists = true, SECURITY_ATTRIBUTES* sa = 0);
  void open(RegistryKey* rootKey, const TCHAR* entry, bool createIfNotExists = true, SECURITY_ATTRIBUTES* sa = 0);
  HKEY getHKEY() const;

  // Creates subkey.
  //
  // Returns true in two cases:
  // 1) subkey exists;
  // 2) subkey didn't exists, and subkey was succerfully created
  //    by method call.
  //
  bool createSubKey(const TCHAR* subkey);
  // Deletes subkey from this registry entry.
  bool deleteSubKey(const TCHAR* subkey);
  // Deletes subkey tree from this registry entry
  bool deleteSubKeyTree(const TCHAR* subkey);
  // Deletes value from this registry entry.
  bool deleteValue(const TCHAR* name);

  // Set value methods
  //

  bool setValueAsInt32(const TCHAR* name, int value);
  bool setValueAsInt64(const TCHAR* name, long value);
  bool setValueAsString(const TCHAR* name, const TCHAR* value);
  bool setValueAsBinary(const TCHAR* name, const void* value,
    size_t sizeInBytes);

  //
  // Get value methods
  //

  bool getValueAsInt32(const TCHAR* name, int* out);
  bool getValueAsInt64(const TCHAR* name, long* out);
  bool getValueAsString(const TCHAR* name, StringStorage* out);
  bool getValueAsBinary(const TCHAR* name, void* value, size_t* sizeInBytes);

  bool getSubKeyNames(StringStorage* subKeyNames, size_t* count);
  bool isOpened();
  void close();

private:
  void initialize(HKEY rootKey, const TCHAR* entry, bool createIfNotExits, SECURITY_ATTRIBUTES* sa);
  DWORD enumKey(DWORD i, StringStorage* name);
  static bool tryOpenSubKey(HKEY key, const TCHAR* subkey, HKEY* openedKey, bool createIfNotExists, SECURITY_ATTRIBUTES* sa);

protected:
  HKEY m_key;
  HKEY m_rootKey;
  StringStorage m_entry;
  friend class Registry;
};

