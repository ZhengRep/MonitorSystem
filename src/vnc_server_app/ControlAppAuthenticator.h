#pragma once
#include "AuthTracker.h"
#include "winSystem/WindowsEvent.h"

class ControlAppAuthenticator: private AuthTracker
{
public:
  ControlAppAuthenticator(UINT64 failureTimeInterval, unsigned int failureMaxCount);
  virtual ~ControlAppAuthenticator();
  bool authenticate(const UINT8 cryptPassword[8], 
    const UINT8 challenge[8], const UINT8 response[8]);
  void breakAndDisableAuthentications();

private:
  void checkBeforeAuth();

  LocalMutex m_authMutex;
  WindowsEvent m_banDelay;
  bool m_isBreaked;
};

