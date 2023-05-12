#pragma once
#include "Utils/CommonHeader.h"
#include "winSystem/WindowsApplication.h"
#include "winSystem/SCMClient.h"
#include "MntService.h"
#include "ServiceControlCommandLine.h"

/**
 * Service control application.
 * Application that can install, remove, start and stop tvnserver service.
 */
class ServiceControlApplication : public WindowsApplication
{
public:
  ServiceControlApplication(HINSTANCE hInstance,
    const TCHAR* windowClassName,
    const TCHAR* commandLine);

  virtual ~ServiceControlApplication();

  virtual int run();

private:
 
  void runElevatedInstance() const throw(SystemException);

  void executeCommand(const ServiceControlCommandLine* cmdLine) const
    throw(SystemException);

  void setMntControlStartEntry() const throw(SystemException);

  void removeMntControlStartEntry() const throw(SystemException);
 
  void reportError(const ServiceControlCommandLine* cmdLine,
    const SCMClientException* ex) const;
 
  void reportError(const ServiceControlCommandLine* cmdLine,
    const SystemException* ex) const;
 
  void reportError(const ServiceControlCommandLine* cmdLine,
    const TCHAR* errorMessage) const;
  
  void reportSuccess(const ServiceControlCommandLine* cmdLine) const;

private:
  
  StringStorage m_commandLine;
};

