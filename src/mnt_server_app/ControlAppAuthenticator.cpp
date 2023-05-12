#include "ControlAppAuthenticator.h"
#include "thread/AutoLock.h"
#include "utils/MntPassCrypt.h"

ControlAppAuthenticator::ControlAppAuthenticator(UINT64 failureTimeInterval, unsigned int failureMaxCount)
  : AuthTracker(failureTimeInterval, failureMaxCount),
  m_isBreaked(false)
{
}

ControlAppAuthenticator::~ControlAppAuthenticator()
{
  breakAndDisableAuthentications();
}

bool ControlAppAuthenticator::authenticate(const UINT8 cryptPassword[8], const UINT8 challenge[8], const UINT8 response[8])
{
  AutoLock al(&m_authMutex);

  checkBeforeAuth();
  if (m_isBreaked) {
    return false;
  }

  MntPassCrypt passCrypt;
  passCrypt.updatePlain(cryptPassword);
  bool isAuthSucceed = passCrypt.challengeAndResponseIsValid(challenge, response);

  if (!isAuthSucceed) {
    notifyAbAuthFailed();
  }

  return isAuthSucceed;
}

void ControlAppAuthenticator::breakAndDisableAuthentications()
{
  m_isBreaked = true;
  m_banDelay.notify();
}

void ControlAppAuthenticator::checkBeforeAuth()
{
  UINT64 banTime = 1;
  while (banTime != 0 && !m_isBreaked) {
    banTime = checkBan();
    if (banTime != 0) {
      m_banDelay.waitForEvent((DWORD)banTime);
    }
  }
}
