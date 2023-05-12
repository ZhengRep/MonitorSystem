#pragma once
#include "utils/inttypes.h"

class MD5
{
public:
  MD5();
  void update(const unsigned char* buf, UINT32 length);
  void update(const char* buf, UINT32 length);
  MD5 finalize();

  //Returns 16 byte MD5 hash
  UINT8* getHash();

private:
  static const int BLOCKSIZE = 64;
  static void decode(UINT32 output[], const UINT8 input[], UINT32 len);
  static void encode(UINT8 output[], const UINT32 input[], UINT32 len);

  void init();
  void transform(const UINT8 block[BLOCKSIZE]);

private:
  static UINT32 F(UINT32 x, UINT32 y, UINT32 z);
  static UINT32 G(UINT32 x, UINT32 y, UINT32 z);
  static UINT32 H(UINT32 x, UINT32 y, UINT32 z);
  static UINT32 I(UINT32 x, UINT32 y, UINT32 z);

  static UINT32 rotateLeft(UINT32 x, int n);

  static void FF(UINT32& a, UINT32 b, UINT32 c, UINT32 d, UINT32 x, UINT32 s, UINT32 ac);
  static void GG(UINT32& a, UINT32 b, UINT32 c, UINT32 d, UINT32 x, UINT32 s, UINT32 ac);
  static void HH(UINT32& a, UINT32 b, UINT32 c, UINT32 d, UINT32 x, UINT32 s, UINT32 ac);
  static void II(UINT32& a, UINT32 b, UINT32 c, UINT32 d, UINT32 x, UINT32 s, UINT32 ac);

private:
  bool m_finalized;
  //Bytes that didn't fit in last 64 byte chunk.
  UINT8 m_buffer[BLOCKSIZE];
  //64bit counter for number of bits(lo, hi)
  UINT32 m_count[2];
  //m_digest so far.
  UINT32 m_state[4];
  UINT8 m_digest[16];
};

