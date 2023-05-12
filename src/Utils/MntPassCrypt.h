#pragma once
#include <vector>
#include "utils/inttypes.h"

class MntPassCrypt
{
public:

  MntPassCrypt();
  virtual ~MntPassCrypt();

  static const int MNT_PASSWORD_SIZE = 8;

  void updatePlain(const UINT8 cryptedPass[MNT_PASSWORD_SIZE]);

  static void getEncryptedPass(UINT8 encryptedPass[MNT_PASSWORD_SIZE],
    const UINT8 plainPassword[MNT_PASSWORD_SIZE]);

  static void getPlainPass(UINT8 plainPassword[MNT_PASSWORD_SIZE],
    const UINT8 encryptedPass[MNT_PASSWORD_SIZE]);

  bool challengeAndResponseIsValid(const UINT8 challenge[16], const UINT8 response[16]);
  void clearPlainPass();

private:
  static const UINT8 m_key[];
  // The vector uses to protect a password from containing in the stack
  // because the stack available for a public on a bug.
  std::vector<UINT8> m_plainPassword;
};

