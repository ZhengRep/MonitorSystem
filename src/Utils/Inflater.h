#pragma once
#include "ZlibBase.h"
#include "ZLibException.h"

class Inflater: public ZLibBase
{
public:
  Inflater();
  ~Inflater();
  void setUnpackedSize(size_t size);

  void inflate() throw(ZLibException);

protected:
  z_stream m_zlibStream;
  size_t m_unpackedSize;
};

