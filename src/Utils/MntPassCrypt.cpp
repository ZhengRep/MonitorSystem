#include "MntPassCrypt.h"
#include "DesCrypt.h"

const UINT8 MntPassCrypt::m_key[] = { 23, 82, 107, 6, 35, 78, 88, 7 };

MntPassCrypt::MntPassCrypt()
  : m_plainPassword(8)
{
}

MntPassCrypt::~MntPassCrypt()
{
  clearPlainPass();
}

void MntPassCrypt::updatePlain(const UINT8 cryptedPass[MNT_PASSWORD_SIZE])
{
  DesCrypt desCrypt;
  desCrypt.decrypt(&m_plainPassword.front(), cryptedPass, m_plainPassword.size(), m_key);
}

void MntPassCrypt::getEncryptedPass(UINT8 encryptedPass[MNT_PASSWORD_SIZE], const UINT8 plainPassword[MNT_PASSWORD_SIZE])
{
  DesCrypt desCrypt;
  desCrypt.encrypt(encryptedPass, plainPassword, MNT_PASSWORD_SIZE, m_key);
}

void MntPassCrypt::getPlainPass(UINT8 plainPassword[MNT_PASSWORD_SIZE], const UINT8 encryptedPass[MNT_PASSWORD_SIZE])
{
  DesCrypt desCrypt;
  desCrypt.decrypt(plainPassword, encryptedPass, MNT_PASSWORD_SIZE, m_key);
}

bool MntPassCrypt::challengeAndResponseIsValid(const UINT8 challenge[16], const UINT8 response[16])
{
  UINT8 cryptedChallenge[16];
  DesCrypt desCrypt;
  desCrypt.encrypt(cryptedChallenge, challenge, sizeof(cryptedChallenge), &m_plainPassword.front());
  if (memcmp(cryptedChallenge, response, sizeof(cryptedChallenge)) == 0) {
    return true;
  } else {
    return false;
  }
}

void MntPassCrypt::clearPlainPass()
{
  memset(&m_plainPassword.front(), 0, m_plainPassword.size());
}

