#pragma once
#include "Utils/CommonHeader.h"
#include "Utils/Exception.h"
#include "SystemException.h"


class SCMClientException : public SystemException {
public:
  const static int ERROR_ALREADY_STOPPED = 0;
  const static int ERROR_STOP_TIMEOUT = 1;
  const static int ERROR_ALREADY_RUNNING = 2;
  const static int ERROR_START_TIMEOUT = 3;
public:

  SCMClientException(int scmErrCode);
  int getSCMErrorCode() const;
private:
  int m_scmErrCode;
};

/**
 * Service client manager client.
 *
 * Gives access to install, remove, start, stop services and queries status
 * of service.
 */
class SCMClient
{
public:
  SCMClient(DWORD desiredAccess = SC_MANAGER_ALL_ACCESS) throw(SystemException);
  virtual ~SCMClient();
  void installService(const TCHAR* name, const TCHAR* nameToDisplay,
    const TCHAR* binPath, const TCHAR* dependencies = _T("")) throw(SystemException);
  void removeService(const TCHAR* name) throw(SystemException);
  void startService(const TCHAR* name, bool waitCompletion = false)
    throw(SystemException, SCMClientException);
  void stopService(const TCHAR* name, bool waitCompletion = false)
    throw(SystemException, SCMClientException);

private:
 
  DWORD getServiceState(SC_HANDLE hService) const throw(SystemException);

private:
  SC_HANDLE m_managerHandle;
};

