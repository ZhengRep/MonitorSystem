#pragma once
#include "Utils/inttypes.h"

class DesCrypt
{
public:
  DesCrypt();
  virtual ~DesCrypt();
  void encrypt(UINT8* dst, const UINT8* src, size_t dataLen, const UINT8* key);
  void decrypt(UINT8* dst, const UINT8* src, size_t dataLen, const UINT8* key);

private:
  enum OperationMode {
    ENCRYPT,
    DECRYPT
  };

  void clearKey();
  void deskey(const UINT8 hexKey[8], OperationMode mode);
  void des(const unsigned char from[8], unsigned char to[8]);

  static void scrunch(const unsigned char* outof, unsigned long* into);
  static void unscrun(const unsigned long* outof, unsigned char* into);
  static void desfunc(unsigned long* block, unsigned long* keys);
  void cookey(register unsigned long* raw1);
  void usekey(register unsigned long* from);


  unsigned long m_knL[32];
};

