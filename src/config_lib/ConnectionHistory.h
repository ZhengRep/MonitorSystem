#pragma once
#include "win_system/RegistryKey.h"
#include <vector>

using namespace std;

class ConnectionHistory
{
public:
  ConnectionHistory(RegistryKey* key, size_t limit);
  virtual ~ConnectionHistory();
  void setLimit(size_t limit);

  size_t getLimit() const;
  void load();
  void save();
  void truncate();
  void clear();
  void addHost(const TCHAR* host);
  size_t getHostCount() const;
  const TCHAR* getHost(size_t i) const;

protected:
  void releaseHosts();
  void removeHost(const TCHAR* host);

protected:
  vector<StringStorage> m_hosts;
  size_t m_limit;
  RegistryKey* m_key;

};

